- # A Dataset of Cache Side-Channel Attacks

  ## About

  Cache side-channel attacks, capable of deducing secret by analyzing timing differences in the shared cache behavior of modern processors, pose a serious security threat.
  
  Flush+Reload and Prime+Probe are two widely-used cache side-channel attacks.
  On the one hand, they can leverage the Last Level Cache (LLC) to break the logical isolation between the attacker and victim, thus can launch cross-core, cross-VM attacks. On the other hand, they can be combined with the new micro-architecture attacks, such as Spectre , leading to more powerful and effective attack variants.
  
  This repository contains four types of cache side-channel attack techniques on the Intel x86-64 architecture, they are as follows:
  
  - [Flush+Reload Family](https://www.usenix.org/system/files/conference/usenixsecurity14/sec14-paper-yarom.pdf)
  - [Prime+Probe Family](https://www.cs.tau.ac.il/~tromer/papers/cache-joc-official.pdf)
  - [Spectre (Flush+Reload based)](https://ieeexplore.ieee.org/abstract/document/8835233)
  - [Spectre (Prime+Probe based)](https://arxiv.org/abs/1802.03802)
  
  ## File Tree
  
  The file tree of our dataset can be shown as follows.  Each folder contains a type of cache side channel attack.
  
  ```
  DATASET
  ├── Flush+Reload
  │   ├── fr*.c
  │	├── fr*.c.out
  │	├── ff*.c
  │	├── ff*.c.out
  │	├── er*.c
  │   └── er*.c.out
  ├── Spectre(Flush+Reload)
  │   ├── fr_spectre*.c
  │   └── fr_spectre*.c.out
  ├── Prime+Probe
  │   ├── pp*.c
  │   └── pp*.c.out
  └── Spectre(Prime+Probe)
      ├── pp_spectre*.c
      └── pp_spectre*.c.out
  ```
  
  There exists 400 samples for each type of attack.  For every attack sample, the mutated source code file and the compiled binary are attached. Note that we compile the source code on Ubuntu 16.04 with GCC/G++ 5.4.0.
  
  ## Flush+Reload Family
  
  Our samples of Flush+Reload are collected from three Github Repos respectively:
  
  - [fr1*.c](https://github.com/IAIK/flush_flush/tree/master/sc/fr)
  
    To compile fr1*.c, use:
  
    `$ g++ -o fr1*.out fr1*.c -lcrypto`  
  
    `$ cp /usr/lib/x86_64-linux-gnu/libcrypto.so ./libcrypto.so `  
  
  - [fr2*.c](https://github.com/nepoche/Flush-Reload)
  
    To obtain the fr2*.out, use:
  
    `$ g++ -o fr2*.out fr2*.c -lcrypto `  
  
    `$ cp /usr/lib/x86_64-linux-gnu/libcrypto.so ./libcrypto.so ` 
  
  - [fr3*.c](https://github.com/0xADE1A1DE/Mastik/blob/main/src/fr.c)
  
    `$ cp fr3*.c /Path/to/Mastik/src/`
  
    `$ cd /Path/to/Mastik/demo/`
  
    `$ gcc FR-1-file-access.c ../src/fr3*.c ../src/timestats.c ../src/vlist.c ../src/util.c -o fr3*.out`
  
  - [ff*.c](https://github.com/IAIK/flush_flush/tree/master/sc/ff)
  
    To compile ff*.c, use:
  
    `$ g++ -o ff*.out ff*.c -lcrypto`
  
    `$ cp /usr/lib/x86_64-linux-gnu/libcrypto.so ./libcrypto.so `  
  
  - er*.c (adapted from [fr1.c](https://github.com/IAIK/flush_flush/tree/master/sc/fr))
  
    To compile er*.c, use:
  
    `$ g++ -o er*.out er*.c -lcrypto`
  
    `$ cp /usr/lib/x86_64-linux-gnu/libcrypto.so ./libcrypto.so `  
  
  ## Prime+Probe Family
  
  Our samples of Prime+Probe are collected from two Github Repos respectively:
  
  - [pp1*.c](https://github.com/IAIK/flush_flush/blob/master/sc/pp/spy.c)
  
    To compile pp1*.c, use:
  
    `$ g++ -o pp1*.out pp1*.c -lcrypto`  
  
    `$ cp /usr/lib/x86_64-linux-gnu/libcrypto.so ./libcrypto.so ` 
  
  - [pp2*.c](https://github.com/jzhang21/prime-probe-attack)
  
    To obtain the pp2*.out, use:
  
    `$ touch transmitted-secret.txt `  
  
    `$ echo "test for pp" > transmitted-secret.txt `  
  
    `$ gcc -o pp2*.out pp2*.c `  
  
  ## Spectre (Flush+Reload based)
  
  Our samples of Flush+Reload with Spectre are collected from the original paper and two Github Repos respectively:
  
  - [fr_spectre1*.c](https://ieeexplore.ieee.org/abstract/document/8835233)
  
    To compile ff_spectre1*.c, use:
  
    `$ gcc fr_spectre1*.c -o ff_spectre1*.out -std=gnu99`  
  
  - [fr_spectre2*.c](https://github.com/opsxcq/exploit-cve-2017-5715/blob/master/exploit.c)
  
    To obtain the ff_spectre2*.out, use:
  
    `$ gcc fr_spectre2*.c -o ff_spectre2*.out -std=gnu99 `  
  
  - [fr_spectre3*.c](https://github.com/idea4good/spectre)
  
    Also, we compile ff_spectre3*.c with following command:
  
    `$ gcc fr_spectre3*.c -o ff_spectre3*.out -std=gnu99`
  
  ## Spectre (Prime+Probe based)
  
  Our samples of Prime+Probe with Spectre are collected from the original paper:
  
  - [pp_spectre*.c](https://arxiv.org/abs/1802.03802)
  
    To compile pp_spectre*.c, use:
  
    `$ gcc pp_spectre*.c -o pp_spectre*.out -pthread -std=gnu99`  
