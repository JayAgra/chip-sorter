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

        uint8_t readOr(uint8_t id, uint8_t fallback) {
            uint8_t readValue = eeprom_store_read(id);
            if (readValue == 0xFF) {
                eeprom_store_safe_write(id, fallback, true);
                return fallback;
            }
            return readValue;
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