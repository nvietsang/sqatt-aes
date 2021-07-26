//
// Created by Nguyen Viet Sang on 12/11/20.
//

#include <stdio.h>
#include "aes.h"
#include "utils.h"

/*
 * Nb: The plaintext and ciphertext blocks consist of Nb 32-bit words
 * Nk: The key consists of Nk 32-bit words
 * Nr: Number of rounds
 */


void key_expansion(uint8_t* round_key, const uint8_t* key, const int Nr){
    // Round 0
    for (int i=0; i<16; i++){
        round_key[i] = key[i];
    }

    // Round 1-9
    int sci; // start column index of old round key
    int lci; // last column index of old round key
    int nci; // index of new column in new round key
    int ri; // i-th round
    uint8_t col[4]; // new column in round key
    for (int i=0; i<=(Nr-1)*4; i=i+4){
        sci = i*4;
        lci = sci + 3*4;

        // RotWord
        col[0] = round_key[lci+1];
        col[1] = round_key[lci+2];
        col[2] = round_key[lci+3];
        col[3] = round_key[lci+0];

        // SubWord
        col[0] = sbox[col[0]];
        col[1] = sbox[col[1]];
        col[2] = sbox[col[2]];
        col[3] = sbox[col[3]];

        // XOR
        col[0] = col[0] ^ round_key[sci+0];
        col[1] = col[1] ^ round_key[sci+1];
        col[2] = col[2] ^ round_key[sci+2];
        col[3] = col[3] ^ round_key[sci+3];

        // Rcon
        ri = i/4 + 1;
        // First column of new round key
        nci = lci + 4;
        round_key[nci+0] = col[0] ^ Rcon[ri];
        round_key[nci+1] = col[1];
        round_key[nci+2] = col[2];
        round_key[nci+3] = col[3];

        for (int j=1; j<=3; j++){
            nci = nci + 4;
            round_key[nci+0] = round_key[nci+0-16] ^ round_key[nci+0-4];
            round_key[nci+1] = round_key[nci+1-16] ^ round_key[nci+1-4];
            round_key[nci+2] = round_key[nci+2-16] ^ round_key[nci+2-4];
            round_key[nci+3] = round_key[nci+3-16] ^ round_key[nci+3-4];
        }
    }
}


void invert_key(uint8_t* round_key, const uint8_t* key, const int Nr){
    // Round 0
    for (int i=0; i<16; i++){
        round_key[i] = key[i];
    }

    // Round 1-9
    int sci; // start column index of old round key
    int lci; // last column index of old round key
    int nci; // index of new column in new round key
    int ri; // i-th round
    uint8_t col[4]; // new column in round key
    for (int i=0; i<=(Nr-1)*4; i=i+4){
        sci = i*4;
        lci = sci + 3*4;

        nci = lci + 4;
        for (int j=1; j<=3; j++){
            nci = nci + 4;
            round_key[nci+0] = round_key[nci+0-16] ^ round_key[nci+0-20];
            round_key[nci+1] = round_key[nci+1-16] ^ round_key[nci+1-20];
            round_key[nci+2] = round_key[nci+2-16] ^ round_key[nci+2-20];
            round_key[nci+3] = round_key[nci+3-16] ^ round_key[nci+3-20];
        }
        // RotWord
        col[0] = round_key[lci+1+16];
        col[1] = round_key[lci+2+16];
        col[2] = round_key[lci+3+16];
        col[3] = round_key[lci+0+16];

        // SubWord
        col[0] = sbox[col[0]];
        col[1] = sbox[col[1]];
        col[2] = sbox[col[2]];
        col[3] = sbox[col[3]];

        // XOR
        col[0] = col[0] ^ round_key[sci+0];
        col[1] = col[1] ^ round_key[sci+1];
        col[2] = col[2] ^ round_key[sci+2];
        col[3] = col[3] ^ round_key[sci+3];

        // Rcon
        ri = i/4 + 1;
        // First column of new round key
        nci = lci + 4;
        round_key[nci+0] = col[0] ^ Rcon[Nr +1 - ri];
        round_key[nci+1] = col[1];
        round_key[nci+2] = col[2];
        round_key[nci+3] = col[3];
    }
}


void add_round_key(uint8_t* state, const uint8_t * round_key, const int round_number){
    int si = 16*round_number; // start index
    for (int i=0; i<16; i++)
        state[i] = state[i] ^ round_key[si+i];
}

void sub_bytes(uint8_t* state){
    for (int i = 0;i < 16; i++)
        {
            state[i] = sbox[state[i]];
        }
}

void shift_row(uint8_t* state){
    uint8_t temp;
    //shift row 1
    temp =  state[1];
    state[1] = state[5];
    state[5]= state[9];
    state[9] = state[13];
    state[13] = temp;

    //shift row 2
    temp =  state[2];
    state[2] = state[10];
    state[10]= temp;

    temp = state[6];
    state[6] = state[14];
    state[14] = temp;

    //shift row 3
    temp = state[3];
    state[3]= state[15];
    state[15]= state[11];
    state[11]= state[7];
    state[7] = temp;

}

void mix_columns(uint8_t* state) {

    uint8_t temp[16];

    for (int i = 0; i < 4; i++) {
        //row 0
        temp[4 * i] = mul2(state[4 * i]) ^ mul3(state[4 * i + 1]) ^ state[4 * i + 2] ^ state[4 * i + 3];

        //row 1
        temp[4 * i + 1] = state[4 * i] ^ mul2(state[4 * i + 1]) ^ mul3(state[4 * i + 2]) ^ state[4 * i + 3];

        //row 2
        temp[4 * i + 2] =  state[4 * i] ^  state[4 * i + 1] ^ mul2(state[4 * i + 2]) ^ mul3(state[4 * i + 3]);

        //row 3
        temp[4 * i + 3] = mul3(state[4 * i]) ^ state[4 * i + 1] ^ state[4 * i + 2] ^  mul2(state[4 * i + 3]);
    }

    for (int  i = 0; i < 16; i++){
        state[i] = temp[i];
    }

}

void invert_mix_columns(uint8_t* state){
    uint8_t temp[16];

    for (int i = 0; i < 4; i++) {
        //row 0

        temp[4 * i] = mul14(state[4 * i]) ^ mul11(state[4 * i + 1]) ^ mul13(state[4 * i + 2]) ^ mul9( state[4 * i + 3]);

        //row 1
        temp[4 * i + 1] = mul9(state[4 * i]) ^ mul14(state[4 * i + 1]) ^ mul11(state[4 * i + 2]) ^ mul13(state[4 * i + 3]);

        //row 2
        temp[4 * i + 2] =  mul13(state[4 * i]) ^  mul9(state[4 * i + 1]) ^ mul14(state[4 * i + 2]) ^ mul11(state[4 * i + 3]);

        //row 3
        temp[4 * i + 3] = mul11(state[4 * i]) ^ mul13(state[4 * i + 1]) ^ mul9(state[4 * i + 2]) ^  mul14(state[4 * i + 3]);
    }

    for (int  i = 0; i < 16; i++){
        state[i] = temp[i];
    }

}

void invert_sub_bytes(uint8_t* state){
    for (int i = 0;i < 16; i++)
    {
        state[i] = isbox[state[i]];
    }

}

void invert_shift_row(uint8_t* state){
    uint8_t temp;
    //inverse shift row 1
    temp =  state[1];
    state[1] = state[13];
    state[13]= state[9];
    state[9] = state[5];
    state[5] = temp;

    //inverse shift row 2
    temp =  state[2];
    state[2] = state[10];
    state[10]= temp;

    temp = state[6];
    state[6] = state[14];
    state[14] = temp;

    //inverse shift row 3
    temp = state[3];
    state[3]= state[7];
    state[7]= state[11];
    state[11]= state[15];
    state[15] = temp;
}

void encrypt(uint8_t *ciphertext, uint8_t* plaintext, uint8_t* key, const int Nr){
    uint8_t round_key[16*(Nr+1)];
    key_expansion(round_key, key, Nr);

    for (int i=0; i<16; i++)
        ciphertext[i] = plaintext[i];

    add_round_key(ciphertext, round_key, 0);

    for(int i = 1; i < Nr ; i++){
        sub_bytes(ciphertext);
        shift_row(ciphertext);
        mix_columns(ciphertext);
        add_round_key(ciphertext, round_key, i);
    }

    sub_bytes(ciphertext);
    shift_row(ciphertext);
    add_round_key(ciphertext,round_key,Nr);

}



/*
 * TEST CASE
 *
 * M = 3243f6a8885a308d313198a2e0370734
 * K = 2b7e151628aed2a6abf7158809cf4f3c
 * Scheme: 16 byte block, 16 byte key
 * C = 3243f6a8885a308d313198a2e0370734
 *
 * R0.k_sch 2b7e15l628aed2a6abf7l58809cf4f3c
 * R1.start 193de3beaOf4e22b9ac68d2ae9f84808
 * R1.sbox  d427llaeeObf98flb8b45de5le4l5230
 * R1.s_row d4bf5d30eOb452aeb84lllflle2798e5
 * R1.m_col 04668le5eOcb199a48f8d37a2806264c
 * R1.k_sch aOfafe1788542cb123a339392a6c7605
 * R2.start a49c7ff2689f352b6b5bea43026a5049
 *
 */
