//
// Created by ASUS on 12/13/2020.
//

#include <stdint.h>
#include "utils.h"
#include <stdio.h>

uint8_t mul2(uint8_t number){
    if (number >= 128)
    {
        return ((uint8_t)(number <<1) ^ 27);
    } else
        return ((uint8_t)(number <<1));

}
uint8_t mul3(uint8_t number){
    return (mul2(number) ^ number);
}

uint8_t  mul9(uint8_t number){
    return(mul2(mul2(mul2(number))) ^ number);
}

uint8_t mul11(uint8_t number){
    return(mul2(mul2(mul2(number))^number) ^number);
}

uint8_t mul13(uint8_t number){
    return (mul2(mul2(mul2(number)^number))^number);
}

uint8_t mul14(uint8_t number){
    return(mul2(mul2(mul2(number)^number)^number));
}

void display_state(const uint8_t *state){
    for (int i=0; i<4; i++){
        printf("%x %x %x %x\n", state[i*4 + 0], state[i*4 + 1], state[i*4 + 2], state[i*4 + 3]);
    }
}

void get_texts_of_lambda_set(uint8_t *out, FILE *fp, int start_line){
    char *line = NULL;
    size_t len = 0;
    unsigned int count = 0;
    unsigned int texts[256*16];
    while ((getline(&line, &len, fp)) != -1){
        if (count >= start_line && count < start_line+256){
//            printf("%d: %s", count%256, line);
            sscanf(line, "%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x \n",
                   &texts[(count % 256) * 16 + 0], &texts[(count % 256) * 16 + 1], &texts[(count % 256) * 16 + 2], &texts[(count % 256) * 16 + 3],
                   &texts[(count % 256) * 16 + 4], &texts[(count % 256) * 16 + 5], &texts[(count % 256) * 16 + 6], &texts[(count % 256) * 16 + 7],
                   &texts[(count % 256) * 16 + 8], &texts[(count % 256) * 16 + 9], &texts[(count % 256) * 16 + 10], &texts[(count % 256) * 16 + 11],
                   &texts[(count % 256) * 16 + 12], &texts[(count % 256) * 16 + 13], &texts[(count % 256) * 16 + 14], &texts[(count % 256) * 16 + 15]);
        }
        count ++;
    }
    for (int i=0; i<256*16; i++)
        out[i] = texts[i];
}

void get_N_lambda_sets(uint8_t *out, FILE *fp, int N){
    char *line = NULL;
    size_t len = 0;
    unsigned int count = 0;
    unsigned int texts[256*16*N];
    while ((getline(&line, &len, fp)) != -1){
        if (count<256*N){
            sscanf(line, "%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x \n",
                   &texts[count * 16 + 0], &texts[count * 16 + 1], &texts[count * 16 + 2], &texts[count * 16 + 3],
                   &texts[count * 16 + 4], &texts[count * 16 + 5], &texts[count * 16 + 6], &texts[count * 16 + 7],
                   &texts[count * 16 + 8], &texts[count * 16 + 9], &texts[count * 16 + 10], &texts[count * 16 + 11],
                   &texts[count * 16 + 12], &texts[count * 16 + 13], &texts[count * 16 + 14], &texts[count * 16 + 15]);
        } else
            break;
        count ++;
    }
    for (int i=0; i<256*16*N; i++)
        out[i] = texts[i];
}
