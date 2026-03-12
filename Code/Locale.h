//
//  Locale.h
//  SLDP
//
//  Created by Jayen Agrawal on 3/9/26.
//

#ifndef LOCALE_H
#define LOCALE_H

#include "StackLabs.h"

#include <avr/pgmspace.h>

static const uint8_t SCREEN_COUNT = 15;

namespace StackLabs {
    namespace Locale {
        /**
         * @enum    Represents a StringID. See full string in Locale.cpp.
        */
        enum StringID : uint8_t {
            S_OUT = 0, S_INV, S_SET, S_OFF_BTN, S_NO, S_YES, S_MENU, S_BLT, S_SLP, S_INF, S_CUR,
            S_LAN, S_MNU, S_TOGGLE, S_OFF_NOW, S_LEFT_ARROW, S_RIGHT_ARROW, S_PAGE_FWD, S_PAGE_BCK,
            S_PG_FWD, S_PG_BCK, S_MENU_TITLE, S_SELECT_CHIPS, S_LONG_PRESS, S_CONFIRM_DISP,
            S_INSUFF_STOCK, S_ONE_OR_MORE, S_CHIPS_UNAVAIL, S_PLEASE_WAIT, S_REMAINING,
            S_CHIP_INVENTORY, S_SETTINGS, S_STATUS, S_VERSION, S_LCD_SETTINGS, S_BACKLIGHT,
            S_SLEEP, S_LOCALE, S_CURRENCY, S_LANGUAGE, S_DEVICE_INFO, S_HARDWARE, S_FIRMWARE,
            S_SOFTWARE, S_PLATFORM, S_FREEMEM, S_UPTIME, S_ERRS, S_VALUES, S_A, S_B, S_SYSTEM_OFF,
            S_GOODBYE, S_SHUTTING_DOWN, S_LCD, S_LOC, S_VAL, STRING_COUNT
        };

        /**
         * @enum    Represents a button.
        */
        struct Button {
            uint8_t stringID, col, width;
        } __attribute__((packed));

        /**
         * @enum    Represents a content row.
        */
        struct Row {
            uint8_t stringID, labelCol, writeCols[4];
        } __attribute__((packed));

        extern const char* const STRINGS[STRING_COUNT] PROGMEM;
        extern const Row ROW_TABLE[SCREEN_COUNT * 3] PROGMEM;
        extern const Button BTN_TABLE[SCREEN_COUNT * 4] PROGMEM;

        /**
         * @brief   Gets a string that is stored in PROGMEM. Just a nice
         *          wrapper for pgm_read_word with an index.
         * 
         * @param   id  String ID to get. Use the enumerator StringID for ease.
         * 
         * @return  The string requested.
        */
        inline const char* getString(uint8_t id) {
            return (const char*)pgm_read_word(&STRINGS[id]);
        }
    }
}

#endif