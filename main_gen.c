//
// Created by Nguyen Viet Sang on 1/5/21.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gen.h"
#include "utils.h"

void write_plaintexts(char *file_out, int Ns){
    uint8_t plaintexts[256*16];
    printf("Writing plaintexts to file...");
    FILE *fp = fopen(file_out, "w");
    if (!fp){
        printf("Cannot open file!\n");
        exit(-1);
    }

    for (int pos=0; pos<Ns; pos++) {
        gen_plaintexts(plaintexts, pos, 0);

        for (int k=0; k<256; k++){
            for (int l=0; l<16; l++)
                fprintf(fp, "%.2x ", plaintexts[16*k + l]);
            fprintf(fp, "\n");
        }
    }

    printf("Done\n");
    fclose(fp);
}

void write_ciphertexts(char *file_out, char *file_in, char *file_key, int Nr, int Ns){
    FILE *fp0 = fopen(file_in, "r");
    if (!fp0){
        printf("Cannot open file!\n");
        exit(-1);
    }

    FILE *fp1 = fopen(file_out, "w");
    if (!fp1){
        printf("Cannot open file!\n");
        exit(-1);
    }

    FILE *fp2 = fopen(file_key, "r");
    if (!fp2){
        printf("Cannot open file!\n");
        exit(-1);
    }
    uint8_t plaintexts[256*16];
    uint8_t ciphertexts[256*16];
    unsigned int keyR[16];
    uint8_t key[16];
    fscanf(fp2, "%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n",
           &keyR[0], &keyR[1], &keyR[2], &keyR[3],
           &keyR[4], &keyR[5], &keyR[6], &keyR[7],
           &keyR[8], &keyR[9], &keyR[10], &keyR[11],
           &keyR[12], &keyR[13], &keyR[14], &keyR[15]);

    for (int i=0; i<16; i++)
        key[i] = keyR[i];

    for (int set=0; set<Ns; set++){
        get_texts_of_lambda_set(plaintexts, fp0, set*256);
        rewind(fp0);
        gen_ciphertexts(ciphertexts, plaintexts, key, Nr);

        for (int k=0; k<256; k++) {
            for (int l=0; l<16; l++)
                fprintf(fp1, "%.2x ", ciphertexts[16*k+l]);
            fprintf(fp1, "\n");
        }
    }

    fclose(fp0);
    fclose(fp1);
    fclose(fp2);
}

int main(int argc, char *argv[]){
    // default values
    char *type = "plaintexts";
    char *file_out = "ciphertexts_4r.txt";
    char *file_in = "plaintexts.txt";
    char *file_key = "key.txt";
    int Ns = 6;
    int Nr = 5;

    if (!strcmp(argv[argc-1], "--help")){
        fprintf(stderr, "Type of generation [plaintexts/ciphertexts], default: %s\n", type);
        fprintf(stderr, "Number of lambda-set, default: 16*%d\n", Ns);
        fprintf(stderr, "Number of rounds, default: %d\n", Nr);
        fprintf(stderr, "Name of output file, default: %s\n", file_out);
        fprintf(stderr, "Name of input file, default: %s\n", file_in);
        fprintf(stderr, "Name of key file, default: %s\n", file_key);
        exit(0);
    }

    if (argc>=2) type       = argv[1];
    if (argc>=3) Ns         = atoi(argv[2]);
    if (argc>=4) Nr         = atoi(argv[3]);
    if (argc>=5) file_out   = argv[4];
    if (argc>=6) file_in    = argv[5];
    if (argc>=7) file_key    = argv[6];

    if (!strcmp(type, "plaintexts"))
        write_plaintexts(file_out, Ns);
    else if (!strcmp(type, "ciphertexts"))
        write_ciphertexts(file_out, file_in, file_key, Nr, Ns);
    else {
        printf("Invalid argument!");
        exit(-1);
    }

    return 0;
}