//
//  ChipCounts.c
//  SLDP
//
//  Created by Jayen Agrawal on 4/7/26.
//

#include "ChipCounts.h"

/*
    Currency ID mapping:
    "USD": 0
    "EUR": 1
    "GBP": 2
    "JPY": 3
    "CHF": 4
*/

uint16_t get_chip_value(uint8_t currency, uint8_t value, uint8_t tube) {
    const uint16_t *valueSet;

    valueSet = (const uint16_t *)pgm_read_word(&(CHIP_VALUES[currency][value]));

    return pgm_read_word(&valueSet[tube]);
}

const char get_currency_symbol(uint8_t currency) {
    if (currency >= 0 && currency < CURRENCY_COUNT) {
        return CURRENCY_SYMS[currency];
    } else {
        return CURRENCY_SYMS[0];
    }
}

const char* get_currency_code(uint8_t currency) {
    if (currency >= 0 && currency < CURRENCY_COUNT) {
        return CURRENCY_CODES[currency];
    } else {
        return CURRENCY_CODES[0];
    }
}