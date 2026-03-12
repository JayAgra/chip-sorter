//
//  LCD.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/9/26.
//

#include "StackLabs.h"
#include "LCD.h"

#include <LiquidCrystal_I2C.h>

static const uint8_t CHAR_EUR[8] PROGMEM = {
    0b00111,
    0b01000,
    0b11110,
    0b01000,
    0b11110,
    0b01000,
    0b00111,
    0b00000
};

static const uint8_t CHAR_GBP[8] PROGMEM = {
    0b00110,
    0b01001,
    0b01000,
    0b11100,
    0b01000,
    0b01000,
    0b11111,
    0b00000
};

static const uint8_t CHAR_JPY[8] PROGMEM = {
    0b10001,
    0b01010,
    0b11111,
    0b00100,
    0b11111,
    0b00100,
    0b00100,
    0b00000
};

static const uint8_t CHAR_CHF[8] PROGMEM = {
    0b01100,
    0b10000,
    0b10000,
    0b11001,
    0b10010,
    0b10010,
    0b10010,
    0b00000
};

static const uint8_t CHAR_LEFT[8] PROGMEM = {
    0b00010,
    0b00110,
    0b01110,
    0b11110,
    0b01110,
    0b00110,
    0b00010,
    0b00000
};

static const uint8_t CHAR_RIGHT[8] PROGMEM = {
    0b01000,
    0b01100,
    0b01110,
    0b01111,
    0b01110,
    0b01100,
    0b01000,
    0b00000
};

static void createChar(
    uint8_t slot,
    const uint8_t* src
) {
    // read from PROGMEM and create
    uint8_t buffer[8];
    memcpy_P(buffer, src, 8);
    lcd.createChar(slot, buffer);
}

static void writePROGMEM(const char* p) {
    // get from PROGMEM
    char c;
    while ((c = pgm_read_byte(p++))) {
        // write!
        lcd.write(c);
    }
}

// can't get length of things in PROGMEM by default, so need this to get length
static uint8_t lenPROGMEM(const char* p) {
    uint8_t n = 0;
    while (pgm_read_byte(p + n)) {
        ++n;
    }
    return n;
}

// create a padded string
static void pad(uint8_t n) {
    for (uint8_t i = 0; i < n; ++i) {
        lcd.write(' ');
    }
}

static void writeButtons(uint8_t btnOffset) {
    // move to third row
    lcd.setCursor(0, 3);

    for (uint8_t i = 0; i < 4; ++i) {
        StackLabs::Locale::Button btn;
        memcpy_P(
            &btn,
            &StackLabs::Locale::BTN_TABLE[btnOffset + i],
            sizeof(btn)
        );
        
        if (btn.width == 0) continue;

        const char* strPtr = StackLabs::Locale::getString(btn.stringID);
        // calculate dimensions of button and pad as needed
        uint8_t labelLen = lenPROGMEM(strPtr);
        uint8_t inner = btn.width - 2;
        uint8_t padding = (inner > labelLen) ? (inner - labelLen) : 0;
        uint8_t leftPad = padding / 2;
        uint8_t rightPad = padding - leftPad;

        // write button
        lcd.setCursor(btn.col, 3);
        lcd.write('[');
        pad(leftPad);
        writePROGMEM(strPtr);
        pad(rightPad);
        lcd.write(']');
    }
}

static void writeRow(
    uint8_t rowOffset,
    uint8_t lcdRow
) {
    // get the row info from PROGMEM
    StackLabs::Locale::Row rd;
    memcpy_P(&rd, &StackLabs::Locale::ROW_TABLE[rowOffset], sizeof(rd));

    // nothing to print, return
    if (rd.stringID == 0xFF) {
        return;
    }

    // set cursor and write from PROGMEM
    lcd.setCursor(rd.labelCol, lcdRow);
    writePROGMEM(StackLabs::Locale::getString(rd.stringID));
}

namespace StackLabs {
    namespace LCD {
        void setup() {
            lcd.init();
            lcd.clear();
            lcd.backlight();
            lcd.setCursor(0, 0);
        }

        void createCharacters() {
            createChar(0, CHAR_EUR);
            createChar(1, CHAR_GBP);
            createChar(2, CHAR_JPY);
            createChar(3, CHAR_CHF);
            createChar(4, CHAR_LEFT);
            createChar(5, CHAR_RIGHT);
        }

        void printEmptyState(uint8_t state) {
            // reject if invalid state
            if (state >= SCREEN_COUNT) return;
            
            // clear and write content & buttons
            lcd.clear();
            for (uint8_t row = 0; row < 3; ++row) {
                writeRow((3 * state) + row, row);
            }
            writeButtons(4 * state);
        }

        void fillValue(
            uint8_t state,
            uint8_t rowIndex,
            uint8_t slotIndex,
            const char* value
        ) {
            if (state >= SCREEN_COUNT) return;
            if (rowIndex >= 3) return;
            if (slotIndex >= 4) return;

            StackLabs::Locale::Row rd;
            memcpy_P(
                &rd,
                &StackLabs::Locale::ROW_TABLE[(state * 3) + rowIndex],
                sizeof(rd)
            );

            uint8_t col = rd.writeCols[slotIndex];
            if (col == 0xFF) return;

            lcd.setCursor(col, rowIndex);
            lcd.print(value);
        }
    }
}