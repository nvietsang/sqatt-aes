//
// Created by Nguyen Viet Sang on 1/21/21.
//

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "aes.h"

#define Nr 5
#define Ns 6

int main(){
    uint8_t plaintext[16];
    uint8_t ciphertext[16];

    uint8_t round_key[16*(Nr+1)];
    uint8_t key0[16];

    uint8_t res_c[16] = {0xa7, 0xd8, 0xf3, 0x1b, 0x9b, 0xa6, 0x79, 0xf9, 0xfd, 0xea, 0x37, 0xc9, 0xa4, 0x6b, 0x35, 0x57};

    uint8_t key5[16] = {0x7a, 0xd6, 0xfc, 0x00, 0x20, 0x6a, 0x00, 0x5b, 0x01, 0x00, 0x00, 0x6a, 0x00, 0xe3, 0xaa, 0x04};

    bool is_key;
    for (int g1=0; g1<256; g1++){
        printf("Guessing (%.2x)\n", g1);
        for (int g2=0; g2<256; g2++){
            for (int g3=0; g3<256; g3++){
                for (int g4=0; g4<256; g4++){
                    is_key = true;
                    key5[3] = g1;
                    key5[6] = g2;
                    key5[9] = g3;
                    key5[12] = g4;
                    invert_key(round_key, key5, Nr);
                    for (int i=0; i<16; i++)
                        key0[i] = round_key[16*Nr + i];

                    encrypt(ciphertext, plaintext, key0, Nr);
                    for (int i=0; i<16; i++){
                        if (ciphertext[i] != res_c[i]) {
                            is_key = false;
                            break;
                        }
                    }
                    if (is_key){
                        printf("Key is (%.2x %.2x %.2x %.2x)\n", g1, g2, g3, g4);
                        exit(0);
                    }
                }
            }
        }
    }


    return 0;
}