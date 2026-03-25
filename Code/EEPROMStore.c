//
//  EEPROMStore.c
//  SLDP
//
//  Created by Jayen Agrawal on 3/25/26.
//

#include "EEPROMStore.h"

/*
    address of data given its id
    right to left:
    converting the id to 16 bit because avr addresses are 16 bits
    casting the address as address of an 8 bit unsigned
*/
#define DATA_ADDRESS(id) ((uint8_t *)(uint16_t)(id))

static bool eeprom_store_exists(uint8_t id) {
    if (id > EEPROM_MAX_ID) return false;

    return eeprom_read_byte(DATA_ADDRESS(id)) != EEPROM_SENTINEL;
}

uint8_t eeprom_store_read(uint8_t id) {
    if (id > EEPROM_MAX_ID) return EEPROM_SENTINEL;
    
    return eeprom_read_byte(DATA_ADDRESS(id));
}

static void eeprom_store_write(uint8_t id, uint8_t value) {
    if (id > EEPROM_MAX_ID) return;

    return eeprom_write_byte(DATA_ADDRESS(id), value);
}

bool eeprom_store_safe_write(
    uint8_t id,
    uint8_t value,
    bool allow_overwrites
) {
    if (id > EEPROM_MAX_ID) return false;
    if (!allow_overwrites && eeprom_store_exists(id)) return false;

    eeprom_store_write(id, value);
    
    return true;
}

bool eeprom_store_delete(uint8_t id) {
    if (id > EEPROM_MAX_ID) return false;

    eeprom_write_byte(DATA_ADDRESS(id), EEPROM_SENTINEL);

    return true;
}
