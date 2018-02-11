# CSE-539 Project 1

Shihao Liu 1213081842

### Desgin:


### Brute Force:
Our idea is every input 32bits with the same key will generate the same output encryption bits, so we have to use a big mapping from all input 32bits  (2^32 integers) to all output 32bits (2^32). If we use in-memory cache, the process will cause 16GB memory, so we use rolling cache to generate 256MB mapping to make comparisons and then generate next 256MB mapping.

Once we find the decryption file, we can start to use `the decryption file` and `the encrytion file` to manipulate the key one by one from `0x00000000` to `0xffffffff` to find the key.

It will take approximate `2 hours` to decrypt `32 bits` and it also take approximate `6 hours` to find the key because generating encryption file cause a lot of I/O.

### Weakness:
We use MD5 to encrypt the key and we know MD5 is one way hash which irreversible and distributed, so we do not have the same weakness as every other ECB encryption applying only shift and xor operations.

 ### How to run our project:
- Ubuntu
- C++11
- Makefile
- Project Link:  https://github.com/Lancher/cse-539/tree/master/project-1
