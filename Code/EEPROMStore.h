//
//  EEPROMStore.h
//  SLDP
//
//  Created by Jayen Agrawal on 3/25/26.
//

#ifndef EEPROMSTORE_H
#define EEPROMSTORE_H

#include <stdbool.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#define EEPROM_MAX_ID 0x3F
#define EEPROM_SENTINEL 0xFF

/**
 * @brief   Checks to see if ID has associated data.
 * 
 * @param   id  ID to check
 * 
 * @return  Boolean indicating if data exists.
*/
static bool eeprom_store_exists(uint8_t id);

/**
 * @brief   Writes data without regard to existing values.
 * 
 * @param   id      ID to write value to.
 * @param   value   Value to write.
*/
static void eeprom_store_write(uint8_t id, uint8_t value);

#ifdef __cplusplus
extern "C" {
#endif
    // please see Data.h StackLabs::Data::read.
    uint8_t eeprom_store_read(uint8_t id);

    // please see Data.h StackLabs::Data::write.
    bool eeprom_store_safe_write(uint8_t id, uint8_t value, bool allow_overwrites);

    // please see Data.h StackLabs::Data::remove.
    bool eeprom_store_delete(uint8_t id);
#ifdef __cplusplus
}
#endif

#endif
