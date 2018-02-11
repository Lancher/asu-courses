#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <climits>
#include <unistd.h>

std::string int_to_hex(unsigned int i);

int main(int argc, char** argv)
{
    unsigned int random_key;
    std::string random_fname = "random.txt";
    std::string encrypt_random_fname = "encrypt-random.txt";
    std::string decrypt_random_fname = "decrypt-random.txt";
    std::string try_encrypt_random_fname = "try-encrypt-random.txt";
    std::string hash_fname = "hash.bin";
    std::string encrypt_hash_fname = "encrypt-hash.bin";
    std::string cmd;
    
    // 1. Generate random key
    std::srand(std::time(nullptr)); 
    random_key = std::rand() % 0x0000ffff;
    std::cout << "1. Generate random key: " << std::endl << int_to_hex(random_key) << std::endl << std::endl;

    // 2. Generate random text file
    std::ofstream f;
    f.open (random_fname);
    f << " ABC";
    f.close();
    std::cout << "2. Generate random text file: " << std::endl << random_fname << std::endl << std::endl;
    
    // 3. Encrypt the random file
    cmd = std::string("./bin/encrypt_decrypt") + " " + "encrypt" + " " + 
        int_to_hex(random_key) + " " + random_fname + " " + encrypt_random_fname + " >> /dev/null";
    system(cmd.c_str());
    std::cout << "3. Encrypt the random file as:" << std::endl << encrypt_random_fname << std::endl << std::endl;

    // 4. Read 4 byte and get the reverse 4 bytes
    std::cout << "4. Start to decrypt the random file as:" << std::endl << encrypt_random_fname << std::endl;
    FILE *encrypt_fd, *decrypt_fd;
    char buf[4];
    unsigned int bytes;
    encrypt_fd = fopen(encrypt_random_fname.c_str(), "rb");
    decrypt_fd = fopen(decrypt_random_fname.c_str(), "wb");

    for (;;) {
        memset (&buf, 0, sizeof(buf));
        bytes = fread(&buf, 1, sizeof(buf), encrypt_fd);
        if (bytes == 0) break;
        unsigned int j = (((unsigned int) buf[0]) << 24) + (((unsigned int) buf[1]) << 16)
             + (((unsigned int) buf[2]) << 8) + ((unsigned int) buf[3]);
        std::cout << "Start to decrypt: " << int_to_hex(j) << std::endl;

        // brute force
        unsigned int found = 0;
        unsigned int limit = 1 << 24;
        unsigned int base = 0;
        char *hash_buf = (char *) std::malloc (limit * 4);

        for (;;) {
            std::cout << "try input binary " << int_to_hex(base) << " to " << int_to_hex(base + limit) << std::endl;

            // generate hash.bin
            for (unsigned int incr = 0; incr < limit; ++incr) {
                unsigned int i = base + incr;
                hash_buf[incr*4] = (i >> 24) & 0x000000ff;
                hash_buf[incr*4+1] = (i >> 16) & 0x000000ff;
                hash_buf[incr*4+2] = (i >> 8) & 0x000000ff;
                hash_buf[incr*4+3] = i & 0x000000ff;
            }
            FILE *fd;
            fd = fopen(hash_fname.c_str(), "wb");
            if (fd == NULL) {
                perror ("fopen()");
                exit (EXIT_FAILURE);
            }
            // fwrite ("12345678", sizeof("12345678"), 1, fd);
            fwrite ((const void*) hash_buf, limit * 4, 1, fd);
            fclose(fd);

            // generate encrypt-hash.bin
            cmd = std::string("./bin/encrypt_decrypt") + " " + "encrypt" + " " + 
                int_to_hex(random_key) + " " + hash_fname + " " + encrypt_hash_fname + " >> /dev/null";
            system(cmd.c_str());

            // get hash i
            fd = fopen(encrypt_hash_fname.c_str(), "rb");
            fread(hash_buf, limit * 4, 1, fd);
            fclose(fd);

            for (unsigned int incr = 0; incr < limit; ++incr) {
                unsigned int i = (((unsigned int) hash_buf[incr*4]) << 24) + (((unsigned int) hash_buf[incr*4+1]) << 16)
                    + (((unsigned int) hash_buf[incr*4+2]) << 8) + ((unsigned int) hash_buf[incr*4+3]);
                if (i == j) {
                    unsigned int k = base + incr;
                    std::cout << "FOUND " << int_to_hex(k) << " <=> " << int_to_hex(i) << std::endl << std::endl;
                    char tmp[4];
                    tmp[0] = (k >> 24) & 0x000000ff;
                    tmp[1] = (k >> 16) & 0x000000ff;
                    tmp[2] = (k >> 8) & 0x000000ff;
                    tmp[3] = k & 0x000000ff;
                    fwrite ((const void*) tmp, bytes, 1, decrypt_fd);
                    found = 1;
                    break;
                }
            }

            // found, then break
            if (found) break;
            
            // reach end, then break
            if (base + limit - 1 == 0xffffffff) break;
            base += limit;
        }
        free(hash_buf);
    }

    // close files
    fclose(decrypt_fd);
    fclose(encrypt_fd);
    std::cout << "Generrate decrypt file:" << std::endl << decrypt_random_fname << std::endl << std::endl;

    // 5. Try to find the key
    std::cout << "5. Try to find the key by " << encrypt_random_fname << " and " << decrypt_random_fname << std::endl;
    
    FILE *fd1, *fd2;
    unsigned int sz = 1024;
    char *buf1 = (char *) std::malloc (sz);
    char *buf2 = (char *) std::malloc (sz);
    unsigned int bytes1, bytes2;

    for (unsigned int i = 0; i <= 0xffffffff; ++i) {
        if (i % (1 << 20) == 0) {
            std::cout << "try binary key from " << int_to_hex(i) << " to " << int_to_hex(i + (1 << 20)) << std::endl;
        }
        
        // generate try-encrypt-random.txt
        cmd = std::string("./bin/encrypt_decrypt") + " " + "encrypt" + " " + 
            int_to_hex(i) + " " + decrypt_random_fname + " " + try_encrypt_random_fname + " >> /dev/null";
        system(cmd.c_str());

        // compare two files
        memset (buf1, 0, sz);
        memset (buf2, 0, sz);
        fd1 = fopen(encrypt_random_fname.c_str(), "rb");
        fd2 = fopen(try_encrypt_random_fname.c_str(), "rb");
        
        for (;;) {
            bytes1 = fread(buf1, 1, sz, fd1);
            bytes2 = fread(buf2, 1, sz, fd2);
                
            if (bytes1 != bytes2) break;
            if (bytes1 == 0) {
                std::cout << "The key is: " << std::endl << int_to_hex(i) << std::endl;
                std::cout << "The original key is: " << std::endl << int_to_hex(random_key) << std::endl;
                free(buf1);
                free(buf2);
                fclose(fd1);
                fclose(fd2);
                exit (EXIT_SUCCESS);
            }

            unsigned int not_same = 0;
            for (unsigned int j = 0; j < bytes1; ++j) { 
                if (buf1[j] != buf2[j]) {
                    not_same = 1;
                    break; 
                }
            }
            if (not_same) break;
        }

        fclose(fd1);
        fclose(fd2);
    }
    free(buf1);
    free(buf2);            
    return 0;
}

std::string int_to_hex(unsigned int i) {
    std::string s = "";
    unsigned int mask = 0x0000000F;
    for (int j = 7; j >= 0; --j) {
        unsigned int tmp = (i >> (j * 4)) & mask;
        if (tmp < 10) { s += std::to_string(tmp);} 
        else if (tmp == 10) { s += "a"; }
        else if (tmp == 11) { s += "b"; }
        else if (tmp == 12) { s += "c"; }
        else if (tmp == 13) { s += "d"; }
        else if (tmp == 14) { s += "e"; }
        else { s += "f"; }
    }
    return s;
}