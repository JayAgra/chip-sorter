//
//  Data.h
//  SLDP
//
//  Created by Jayen Agrawal on 3/25/26.
//

#ifndef DATA_H
#define DATA_H

#include "EEPROMStore.h"

namespace StackLabs {
    namespace Data {
        /**
         * @brief   Read the value stored in EEPROM at id.
         * 
         * @param   id  ID of data, [0, 63].
         * 
         * @return  Value of data. 0xFF if no data.
        */
        uint8_t read(uint8_t id);

        /**
         * @brief   Writes data to EEPROM at an ID.
         * 
         * @param   id              ID to write at.
         * @param   value           u8 value to be written. Note 0xFF is the
         *                          sentinel, so its equivalent to deleting.
         * @param   allowOverwrites Allow overwriting existing values?
         * 
         * @return  Success boolean. Fails if index out of [0, 63] or if the ID
         *          already had data and allowOverwrites was false.
        */
        bool write(uint8_t id, uint8_t value, bool allowOverwrites);

        /**
         * @brief   Removes data with given ID. Performs a write, placing the
         *          0xFF sentinel value in EEPROM at the given location.
         * 
         * @param   id  ID of data to remove.
         * 
         * @return  Success boolean. Fails if out of [0, 63] range.
        */
        bool remove(uint8_t id);

        /**
         * @brief   Sets all 63 possible values to 0xFF, the sentinel.
        */
        void reset();
    }
}

#endif