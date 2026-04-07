//
//  ChipCounts.h
//  SLDP
//
//  Created by Jayen Agrawal on 4/7/26.
//

#ifndef CHIPCOUNTS_H
#define CHIPCOUNTS_H

#include <avr/pgmspace.h>

#define CURRENCY_COUNT 5

static const char CURRENCY_SYMS[]  = { '$', '\x01', '\x02', '\x03', '\x04' };
static const char* const CURRENCY_CODES[] = { "USD", "EUR", "GBP", "JPY", "CHF" };

static const uint16_t valueSet_005_010_025_100[4] PROGMEM = {5, 10, 25, 100};
static const uint16_t valueSet_005_010_025_050[4] PROGMEM = {5, 10, 25, 50};
static const uint16_t valueSet_500_01K_05K_10K[4] PROGMEM = {500, 1000, 5000, 10000};
static const uint16_t valueSet_100_200_01K_02K[4] PROGMEM = {100, 200, 1000, 2000};

const uint16_t *const CHIP_VALUES[5][2] PROGMEM = {
    {valueSet_005_010_025_100, valueSet_005_010_025_050},
    {valueSet_005_010_025_050, valueSet_005_010_025_100},
    {valueSet_005_010_025_050, valueSet_005_010_025_100},
    {valueSet_500_01K_05K_10K, valueSet_100_200_01K_02K},
    {valueSet_005_010_025_050, valueSet_005_010_025_100},
};

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief   Returns numerical value of a chip based on given system.
 * 
 * @param   currency    Currency being used.
 * @param   value       Value system being used (0 or 1).
 * @param   tube        Tube/color of chip in question (R, G, Blu, Blk).
 * 
 * @return  Value of chip.
*/
uint16_t get_chip_value(uint8_t currency, uint8_t value, uint8_t tube);

/**
 * @brief   Returns currency symbol character for given currency.
 * 
 * @param   currency    Currency ID.
 * 
 * @return  Currency symbol character.
*/
const char get_currency_symbol(uint8_t currency);

/**
 * @brief   Returns currency code string for given currency.
 * 
 * @param   currency    Currency ID.
 * 
 * @return  Currency code string.
*/
const char* get_currency_code(uint8_t currency);
#ifdef __cplusplus
}
#endif

#endif