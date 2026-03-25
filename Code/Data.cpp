//
//  Data.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/25/26.
//

#include "Data.h"

namespace StackLabs {
    namespace Data {
        uint8_t read(uint8_t id) {
            return eeprom_store_read(id);
        }

        bool write(uint8_t id, uint8_t value, bool allowOverwrites) {
            return eeprom_store_safe_write(id, value, allowOverwrites);
        }

        bool remove(uint8_t id) {
            return eeprom_store_delete(id);
        }

        void reset() {
            for (uint8_t i = 0; i <= EEPROM_MAX_ID; ++i) {
                eeprom_store_delete(i);
            }
        }
    }
}