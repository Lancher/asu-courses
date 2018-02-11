### Requirements
1. C++ std11
2. Makefile

### Create binaries
```
>> make clean && make
```

### Encrypt the file
```
./bin/encrypt_decrypt encrypt 12345678 test.pdf test_encrypt.pdf
```

### Decrypt the file
```
./bin/encrypt_decrypt decrypt 12345678 test.pdf test_decrypt.pdf
```

### Brute force
Brute force will do the following steps:
1. Generate random key  (show in terminal).
2. Generate random text file `random.txt`.
3. Use the key and file above to create encrypt file `encrypt-random.txt`.
4. Start to decrypt and then generate decrypt file `decrypt-random.txt`.
5. Start use the `decrypt-random.txt` and `encrypt-random.txt` to get the random key (show in terminal).
```
./bin/bruteforce
```

