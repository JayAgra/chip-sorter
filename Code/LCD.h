//
//  LCD.h
//  SLDP
//
//  Created by Jayen Agrawal on 3/9/26.
//

#ifndef LCD_H
#define LCD_H

#include "StackLabs.h"

#include <LiquidCrystal_I2C.h>

static LiquidCrystal_I2C lcd(0x27, 20, 4);

namespace StackLabs {
    namespace LCD {
        /**
         * @brief   Sets up the display and enables the backlight.
        */
        void setup();

        /**
         * @brief   Creates the custom characters used for currency support
         *          and as a few UI elements. Requires prepareDisplay to be
         *          called first in order to be successful.
        */
        void createCharacters();

        /**
         * @brief   Prints a blank UI according to the state ID. Please see
         *          ui.txt for the state IDs.
         * 
         * @param   state   State ID, according to ui.txt.
        */
        void printEmptyState(uint8_t state);

        /**
         * @brief   Fills a value into the blank UI. I have not yet implemented
         *          any of the safety things, but it should work. Need to write
         *          code to reference the correct insert type & length.
         * 
         * @param   state   The state that it should be written to. Note that I
         *                  have not yet written the safeties so this kinda
         *                  just goes unchecked, so make sure it is right for
         *                  the time being.
         * @param   row     Which row is being filled? Same w/ above on safety.
         * @param   slot    Which slot is being filled? Same on safety.
         * @param   value   What needs to be filled?
        */
        void fillValue(
            uint8_t state,
            uint8_t row,
            uint8_t slot,
            const char* value
        );
    }
}

#endif