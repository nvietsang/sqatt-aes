#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "aes.h"
#include "gen.h"
#include "utils.h"

#define Nr 5 // Number of rounds
#define Ns 6

bool attack_5rounds_1byte(unsigned int qid,
                           unsigned int uidx1, unsigned int gidx2,
                           unsigned int gidx3, unsigned int gidx4,
                           uint8_t *key5, const uint8_t *ciphertexts){
    uint8_t a[16];
    uint8_t r;
    qid = qid * 4;

    bool is_key;
    for (int guess_byte1 = 0; guess_byte1 < 256; guess_byte1++){
        key5[uidx1]     = guess_byte1;

        printf("Guessing (%.2x)...\n", guess_byte1);
        for (int guess_byte_K4 = 0; guess_byte_K4 < 256; guess_byte_K4++){
            is_key = true;
            for (int li=0; li<Ns; li++){
                r = 0;
                for(int ci = 0; ci < 256; ci++){
                    a[uidx1]     = (uint8_t) key5[uidx1]    ^ ciphertexts[li * 256 * 16 + ci * 16 + uidx1];
                    a[gidx2]     = (uint8_t) key5[gidx2]    ^ ciphertexts[li * 256 * 16 + ci * 16 + gidx2];
                    a[gidx3]     = (uint8_t) key5[gidx3]    ^ ciphertexts[li * 256 * 16 + ci * 16 + gidx3];
                    a[gidx4]     = (uint8_t) key5[gidx4]    ^ ciphertexts[li * 256 * 16 + ci * 16 + gidx4];

                    invert_shift_row(a);
                    invert_sub_bytes(a);
                    invert_mix_columns(a);
                    a[qid] = (uint8_t) guess_byte_K4 ^ a[qid];
                    invert_shift_row(a);
                    invert_sub_bytes(a);
                    r = r ^ a[qid];
                }
                if (r != 0){
                    is_key = false;
                    break;
                }
            }
            if (is_key){
                printf("Key is: (%.2x)\n", guess_byte1);
                return true;
            }
        }
    }
    return false;
}

bool attack_5rounds_2bytes(unsigned int qid,
                           unsigned int uidx1, unsigned int uidx2,
                           unsigned int gidx3, unsigned int gidx4,
                           uint8_t *key5, const uint8_t *ciphertexts){
    uint8_t a[16];
    uint8_t r;
    qid = qid * 4;

    bool is_key;
    for (int guess_byte1 = 0; guess_byte1 < 256; guess_byte1++){
        for (int guess_byte2 = 0; guess_byte2 < 256; guess_byte2++){
            key5[uidx1]     = guess_byte1;
            key5[uidx2]     = guess_byte2;

            printf("Guessing (%.2x %.2x)...\n", guess_byte1, guess_byte2);
            for (int guess_byte_K4 = 0; guess_byte_K4 < 256; guess_byte_K4++){
                is_key = true;
                for (int li=0; li<Ns; li++){
                    r = 0;
                    for(int ci = 0; ci < 256; ci++){
                        a[uidx1]     = (uint8_t) key5[uidx1]    ^ ciphertexts[li * 256 * 16 + ci * 16 + uidx1];
                        a[uidx2]     = (uint8_t) key5[uidx2]    ^ ciphertexts[li * 256 * 16 + ci * 16 + uidx2];
                        a[gidx3]     = (uint8_t) key5[gidx3]    ^ ciphertexts[li * 256 * 16 + ci * 16 + gidx3];
                        a[gidx4]     = (uint8_t) key5[gidx4]    ^ ciphertexts[li * 256 * 16 + ci * 16 + gidx4];

                        invert_shift_row(a);
                        invert_sub_bytes(a);
                        invert_mix_columns(a);
                        a[qid] = (uint8_t) guess_byte_K4 ^ a[qid];
                        invert_shift_row(a);
                        invert_sub_bytes(a);
                        r = r ^ a[qid];
                    }
                    if (r != 0){
                        is_key = false;
                        break;
                    }
                }
                if (is_key){
                    printf("Key is: (%.2x %.2x)\n", guess_byte1, guess_byte2);
                    return true;
                }
            }
        }
    }
    return false;
}

bool attack_5rounds_3bytes(unsigned int qid,
                           unsigned int uidx1, unsigned int uidx2,
                           unsigned int uidx3, unsigned int gidx4,
                           uint8_t *key5, const uint8_t *ciphertexts){
    uint8_t a[16];
    uint8_t r;
    qid = qid * 4;

    bool is_key;
    for (int guess_byte1 = 0; guess_byte1 < 256; guess_byte1++){
        for (int guess_byte2 = 0; guess_byte2 < 256; guess_byte2++){
            for (int guess_byte3 = 0; guess_byte3 < 256; guess_byte3++){
                key5[uidx1]     = guess_byte1;
                key5[uidx2]     = guess_byte2;
                key5[uidx3]     = guess_byte3;

                printf("Guessing (%.2x %.2x %.2x)...\n", guess_byte1, guess_byte2, guess_byte3);
                for (int guess_byte_K4 = 0; guess_byte_K4 < 256; guess_byte_K4++){
                    is_key = true;
                    for (int li=0; li<Ns; li++){
                        r = 0;
                        for(int ci = 0; ci < 256; ci++){
                            a[uidx1]     = (uint8_t) key5[uidx1]    ^ ciphertexts[li * 256 * 16 + ci * 16 + uidx1];
                            a[uidx2]     = (uint8_t) key5[uidx2]    ^ ciphertexts[li * 256 * 16 + ci * 16 + uidx2];
                            a[uidx3]     = (uint8_t) key5[uidx3]    ^ ciphertexts[li * 256 * 16 + ci * 16 + uidx3];
                            a[gidx4]     = (uint8_t) key5[gidx4]    ^ ciphertexts[li * 256 * 16 + ci * 16 + gidx4];

                            invert_shift_row(a);
                            invert_sub_bytes(a);
                            invert_mix_columns(a);
                            a[qid] = (uint8_t) guess_byte_K4 ^ a[qid];
                            invert_shift_row(a);
                            invert_sub_bytes(a);
                            r = r ^ a[qid];
                        }
                        if (r != 0){
                            is_key = false;
                            break;
                        }
                    }
                    if (is_key){
                        printf("Key is: (%.2x %.2x %.2x)\n", guess_byte1, guess_byte2, guess_byte3);
                        return true;
                    }
                }
            }

        }
    }
    return false;
}

bool attack_5rounds_4bytes(unsigned int qid,
                           unsigned int uidx1, unsigned int uidx2,
                           unsigned int uidx3, unsigned int uidx4,
                           uint8_t *key5, const uint8_t *ciphertexts){
    uint8_t a[16];
    uint8_t r;
    qid = qid * 4;

    bool is_key;
    for (int guess_byte1 = 0; guess_byte1 < 256; guess_byte1++){
        for (int guess_byte2 = 0; guess_byte2 < 256; guess_byte2++){
            for (int guess_byte3 = 0; guess_byte3 < 256; guess_byte3++){
                for (int guess_byte4 = 0; guess_byte4 < 256; guess_byte4++){
                    key5[uidx1]     = guess_byte1;
                    key5[uidx2]     = guess_byte2;
                    key5[uidx3]     = guess_byte3;
                    key5[uidx4]     = guess_byte4;

                    printf("Guessing (%.2x %.2x %.2x %.2x)...\n", guess_byte1, guess_byte2, guess_byte3, guess_byte4);
                    for (int guess_byte_K4 = 0; guess_byte_K4 < 256; guess_byte_K4++){
                        is_key = true;
                        for (int li=0; li<Ns; li++){
                            r = 0;
                            for(int ci=0; ci<256; ci++){
                                a[uidx1]     = (uint8_t) key5[uidx1]    ^ ciphertexts[li * 256 * 16 + ci * 16 + uidx1];
                                a[uidx2]     = (uint8_t) key5[uidx2]    ^ ciphertexts[li * 256 * 16 + ci * 16 + uidx2];
                                a[uidx3]     = (uint8_t) key5[uidx3]    ^ ciphertexts[li * 256 * 16 + ci * 16 + uidx3];
                                a[uidx4]     = (uint8_t) key5[uidx4]    ^ ciphertexts[li * 256 * 16 + ci * 16 + uidx4];

                                invert_shift_row(a);
                                invert_sub_bytes(a);
                                invert_mix_columns(a);
                                a[qid] = (uint8_t) guess_byte_K4 ^ a[qid];
                                invert_shift_row(a);
                                invert_sub_bytes(a);
                                r = r ^ a[qid];
                            }
                            if (r != 0){
                                is_key = false;
                                break;
                            }
                        }
                        if (is_key){
                            printf("Key is: (%.2x %.2x %.2x %.2x)\n", guess_byte1, guess_byte2, guess_byte3, guess_byte4);
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

int main(int argc, char *argv[]){
    char *file_in = "ciphertexts_5r.txt";

    if (!strcmp(argv[argc-1], "--help")){
        fprintf(stderr, "Path to file of ciphertexts, default: %s\n", file_in);
    }

    if (argc>=2) file_in = argv[1];

    uint8_t key5[16] = {0x8a, 0xbd, 0x00, 0x00, 0xa2, 0x32, 0x00, 0x01, 0x12, 0x00, 0x25, 0x00, 0x86, 0x56, 0xab, 0x00};

    FILE *fp = fopen(file_in, "r");
    if (!fp){
        printf("Cannot open file!\n");
        exit(-1);
    }

    uint8_t *ciphertexts = (uint8_t *) malloc(256*16*Ns*sizeof(uint8_t));
    get_N_lambda_sets(ciphertexts, fp, Ns);

    unsigned int qid, idx1, idx2, idx3, idx4;
    qid = 0; idx1 = 7; idx2 = 10; idx3 = 13; idx4 = 0; // quadruplet 1
    attack_5rounds_2bytes(qid, idx1, idx2, idx3, idx4, key5, ciphertexts);

    qid = 1; idx1 = 11; idx2 = 14; idx3 = 1; idx4 = 4; // quadruplet 2
    attack_5rounds_2bytes(qid, idx1, idx2, idx3, idx4, key5, ciphertexts);

    qid = 2; idx1 = 15; idx2 = 2; idx3 = 5; idx4 = 8; // quadruplet 3
    attack_5rounds_3bytes(qid, idx1, idx2, idx3, idx4, key5, ciphertexts);

    qid = 3; idx1 = 3; idx2 = 6; idx3 = 9; idx4 = 12; // quadruplet 4
    attack_5rounds_4bytes(qid, idx1, idx2, idx3, idx4, key5, ciphertexts);

    uint8_t round_key[16 * (Nr + 1)];
    invert_key(round_key, key5, Nr);
    for (int i=0; i<=Nr; i++){
        printf("Round Key %d\n", Nr - i);
        printf("%.2x %.2x %.2x %.2x\n", round_key[i * 16 + 0], round_key[i * 16 + 4], round_key[i * 16 + 8], round_key[i * 16 + 12]);
        printf("%.2x %.2x %.2x %.2x\n", round_key[i * 16 + 1], round_key[i * 16 + 5], round_key[i * 16 + 9], round_key[i * 16 + 13]);
        printf("%.2x %.2x %.2x %.2x\n", round_key[i * 16 + 2], round_key[i * 16 + 6], round_key[i * 16 + 10], round_key[i * 16 + 14]);
        printf("%.2x %.2x %.2x %.2x\n", round_key[i * 16 + 3], round_key[i * 16 + 7], round_key[i * 16 + 11], round_key[i * 16 + 15]);
    }

    return 0;
}
