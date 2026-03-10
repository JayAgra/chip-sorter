//
//  LCD.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/9/26.
//

#include "StackLabs.h"
#include "LCD.h"
#include <LiquidCrystal_I2C.h>

uint8_t EURO_SYMBOL[8] = {
    0b00111,
    0b01000,
    0b11110,
    0b01000,
    0b11110,
    0b01000,
    0b00111,
    0b00000
};

uint8_t POUND_SYMBOL[8] = {
    0b00110,
    0b01001,
    0b01000,
    0b11100,
    0b01000,
    0b01000,
    0b11111,
    0b00000
};

uint8_t YEN_SYMBOL[8] = {
    0b10001,
    0b01010,
    0b11111,
    0b00100,
    0b11111,
    0b00100,
    0b00100,
    0b00000
};

uint8_t FRANC_SYMBOL[8] = {
    0b01100,
    0b10000,
    0b10000,
    0b11001,
    0b10010,
    0b10010,
    0b10010,
    0b00000
};

uint8_t LEFT_ARROW[8] = {
    0b00010,
    0b00110,
    0b01110,
    0b11110,
    0b01110,
    0b00110,
    0b00010,
    0b00000
};

uint8_t RIGHT_ARROW[8] = {
    0b01000,
    0b01100,
    0b01110,
    0b01111,
    0b01110,
    0b01100,
    0b01000,
    0b00000
};

namespace StackLabs {
    namespace LCD {
        void prepareDisplay(LiquidCrystal_I2C &lcd) {
            lcd.init();
            lcd.clear();
            lcd.backlight();
        }

        void createCharacters(LiquidCrystal_I2C &lcd) {
            lcd.createChar(0, EURO_SYMBOL);
            lcd.createChar(1, POUND_SYMBOL);
            lcd.createChar(2, YEN_SYMBOL);
            lcd.createChar(3, FRANC_SYMBOL);
            lcd.createChar(4, LEFT_ARROW);
            lcd.createChar(5, RIGHT_ARROW);
        }
    }
}