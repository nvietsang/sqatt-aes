//
// Created by Nguyen Viet Sang on 12/26/20.
//

#include "gen.h"
#include "aes.h"
#include <stdio.h>

void gen_plaintexts(uint8_t *plaintexts, int position, uint8_t inactive){
    for (int i = 0; i < 256; i++){
        for (int j = 0; j < 16; j++){
            if (j == position) {
                plaintexts[16 * i + j] = i;
            } else
                plaintexts[16*i +j ] = inactive;
        }

    }
}

void gen_ciphertexts(uint8_t *ciphertexts, const uint8_t *plaintexts, uint8_t* key, const int Nr){
    uint8_t cipher[16];
    uint8_t plain[16];

    for (int i = 0; i < 256; i++){
        for(int j = 0; j < 16; j++) {
            plain[j] = plaintexts[16 * i + j];
        }
        encrypt(cipher, plain, key, Nr );
        for (int j = 0; j < 16; j++){
            ciphertexts[16*i + j] = cipher[j];
        }
    }
}
