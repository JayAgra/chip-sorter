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

namespace StackLabs {
    namespace LCD {
        /**
         * @brief   Initializes the display and enables the backlight.
         * 
         * @param   lcd     LCD to prepare.
        */
        void prepareDisplay(LiquidCrystal_I2C &lcd);

        /**
         * @brief   Creates the custom characters used for currency support
         *          and as a few UI elements. Requires prepareDisplay to be
         *          called first in order to be successful.
         * 
         * @param   lcd     LCD to write custom characters to.
        */
        void createCharacters(LiquidCrystal_I2C &lcd);
    }
}

#endif