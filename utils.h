//
// Created by ASUS on 12/13/2020.
//
#include <stdio.h>

#ifndef SQUARE_ATTACK_UTILS_H
#define SQUARE_ATTACK_UTILS_H

uint8_t mul2(uint8_t number);
uint8_t mul3(uint8_t number);
uint8_t mul9(uint8_t number);
uint8_t mul11(uint8_t number);
uint8_t mul13(uint8_t number);
uint8_t mul14(uint8_t number);
void display_state(const uint8_t *state);
void get_texts_of_lambda_set(uint8_t *texts, FILE *fp, int start_line);
void get_N_lambda_sets(uint8_t *out, FILE *fp, int N);

#endif //SQUARE_ATTACK_UTILS_H
