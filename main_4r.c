#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "aes.h"
#include "gen.h"
#include "utils.h"

#define Nr 4
#define Ne 10
#define NONE 999
#define Ns 2

/*
 *
 *      for each guess byte in range 0-255:
 *          r = 0
 *          for each cipher in 256 ciphertexts:
 *              a = f(d)
 *              r = r xor a
 *          if r == 0:
 *              choose the guess byte of K4
 */
void attack_4rounds(uint8_t *key4, const uint8_t *ciphertexts){
    uint8_t a[16];
    uint8_t r[16];
    bool is_key[16];

    for(int guess = 0; guess < 256; guess++){
        for (int i=0; i<16; i++)
            is_key[i] = true;
        for (int li=0; li<Ns; li++){
            for(int i=0; i<16; i++)
                r[i] = 0;
            for(int ci=0; ci<256; ci++){
                for (int j=0; j<16; j++)
                    a[j] = (uint8_t) guess ^ ciphertexts[li * 256 * 16 + ci * 16 + j];

                invert_shift_row(a);
                invert_sub_bytes(a);

                // AddRoundKey at round 4
                for (int i=0; i<16; i++)
                    r[i] = r[i] ^ a[i];
            }
            shift_row(r); // turn back to the right positions of K4
            // check if balanced cells
            for (int i=0; i<16; i++){
                if (r[i] != 0)
                    is_key[i] = false;
            }
        }
        for (int i=0; i<16; i++){
            if (is_key[i])
                key4[i] = guess;
        }
    }
}

int main(int argc, char *argv[]) {
    char *file_in = "ciphertexts_4r.txt";
    if (!strcmp(argv[argc-1], "--help")){
        fprintf(stderr, "Path to file of ciphertexts, default: %s\n", file_in);
    }
    if (argc>=2) file_in = argv[1];

    FILE *fp = fopen(file_in, "r");
    if (!fp){
        printf("Cannot open file!\n");
        exit(-1);
    }

    uint8_t *ciphertexts = (uint8_t *) malloc(256*16*Ns*sizeof(uint8_t));
    get_N_lambda_sets(ciphertexts, fp, Ns);

    uint8_t key4[16];
    clock_t start, end;
    start = clock();
    attack_4rounds(key4, ciphertexts);
    end = clock();
    printf("Attacking time: %.4f\n", (double)(end-start)/CLOCKS_PER_SEC);

    uint8_t round_key[16*(Nr+1)];
    invert_key(round_key, key4, Nr);
    for (int i=0; i<=Nr; i++){
        printf("Key at round %d\n", Nr - i);
        printf("%.2x %.2x %.2x %.2x\n", round_key[i * 16 + 0], round_key[i * 16 + 4], round_key[i * 16 + 8], round_key[i * 16 + 12]);
        printf("%.2x %.2x %.2x %.2x\n", round_key[i * 16 + 1], round_key[i * 16 + 5], round_key[i * 16 + 9], round_key[i * 16 + 13]);
        printf("%.2x %.2x %.2x %.2x\n", round_key[i * 16 + 2], round_key[i * 16 + 6], round_key[i * 16 + 10], round_key[i * 16 + 14]);
        printf("%.2x %.2x %.2x %.2x\n", round_key[i * 16 + 3], round_key[i * 16 + 7], round_key[i * 16 + 11], round_key[i * 16 + 15]);
    }

    return 0;
}
