//
// Created by Nguyen Viet Sang on 12/26/20.
//

#include <stdint.h>

#ifndef SQUARE_ATTACK_GEN_H
#define SQUARE_ATTACK_GEN_H

void gen_plaintexts(uint8_t *plaintexts, int position, uint8_t inactive);
void gen_ciphertexts(uint8_t *ciphertexts, const uint8_t *plaintexts, uint8_t* key, const int Nr);

#endif //SQUARE_ATTACK_GEN_H
