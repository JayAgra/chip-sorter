//
//  Locale.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/9/26.
//

// i kinda deleted multi language support but ill fix that later

#include "Locale.h"

#include <avr/pgmspace.h>

//         STRING ID                  STRING
static const char _OUT[]           PROGMEM = "OUT";
static const char _INV[]           PROGMEM = "INV";
static const char _SET[]           PROGMEM = "SET";
static const char _OFF_BTN[]       PROGMEM = "OFF";
static const char _NO[]            PROGMEM = "NO!";
static const char _YES[]           PROGMEM = "YES";
static const char _MENU[]          PROGMEM = "MENU";
static const char _BLT[]           PROGMEM = "BLT";
static const char _SLP[]           PROGMEM = "SLP";
static const char _INF[]           PROGMEM = "INF";
static const char _CUR[]           PROGMEM = "CUR";
static const char _LAN[]           PROGMEM = "LAN";
static const char _MNU[]           PROGMEM = "MNU";
static const char _TOGGLE[]        PROGMEM = "TOGGLE";
static const char _OFF_NOW[]       PROGMEM = "OFF NOW";
static const char _LEFT_ARROW[]    PROGMEM = "\x04""";
static const char _RIGHT_ARROW[]   PROGMEM = "\x05""";
static const char _PAGE_FWD[]      PROGMEM = "PAGE \x05""";
static const char _PAGE_BCK[]      PROGMEM = "\x04"" PAGE";
static const char _PG_FWD[]        PROGMEM = "PG\x05""";
static const char _PG_BCK[]        PROGMEM = "\x04""PG";
static const char _MENU_TITLE[]    PROGMEM = "MENU";
static const char _SELECT[]        PROGMEM = "SELECT CHIPS";
static const char _LONG_PRESS[]    PROGMEM = "LONG PRESS TO REVIEW";
static const char _CONFIRM[]       PROGMEM = "CONFIRM DISP";
static const char _INSUFF[]        PROGMEM = "INSUFFICIENT STOCK";
static const char _ONE_OR_MORE[]   PROGMEM = "ONE OR MORE RQSTD.";
static const char _UNAVAIL[]       PROGMEM = "CHIPS UNAVAILABLE!";
static const char _PLEASE_WAIT[]   PROGMEM = "PLEASE WAIT...";
static const char _REMAINING[]     PROGMEM = "REMAINING:";
static const char _INVENTORY[]     PROGMEM = "CHIP INVENTORY";
static const char _SETTINGS[]      PROGMEM = "SETTINGS";
static const char _STATUS[]        PROGMEM = "STATUS:";
static const char _VERSION[]       PROGMEM = "VER:";
static const char _LCD_SET[]       PROGMEM = "LCD SETTINGS";
static const char _BACKLIGHT[]     PROGMEM = "BACKLIGHT:";
static const char _SLEEP[]         PROGMEM = "SLEEP:";
static const char _LOCALE[]        PROGMEM = "LOCALE";
static const char _CURRENCY[]      PROGMEM = "CURRENCY:";
static const char _LANGUAGE[]      PROGMEM = "LANGUAGE:";
static const char _DEVINFO[]       PROGMEM = "DEVICE INFO PG";
static const char _HARDWARE[]      PROGMEM = "HARDWARE:";
static const char _FIRMWARE[]      PROGMEM = "FIRMWARE:";
static const char _SOFTWARE[]      PROGMEM = "SOFTWARE:";
static const char _PLATFORM[]      PROGMEM = "PLATFORM:";
static const char _FREEMEM[]       PROGMEM = "FREEMEM:";
static const char _UPTIME[]        PROGMEM = "UPTIME:";
static const char _ERRS[]          PROGMEM = "ERRS:";
static const char _VALUES[]        PROGMEM = "VALUES";
static const char _A[]             PROGMEM = "A:";
static const char _B[]             PROGMEM = "B:";
static const char _SYSTEM_OFF[]    PROGMEM = "SYSTEM OFF";
static const char _GOODBYE[]       PROGMEM = "GOODBYE!";
static const char _SHUTTING_DOWN[] PROGMEM = "SHUTTING DOWN:";
static const char _LCD[]           PROGMEM = "LCD";
static const char _LOC[]           PROGMEM = "LOC";
static const char _VAL[]           PROGMEM = "VAL";

static const char* const StackLabs::Locale::STRINGS[STRING_COUNT] PROGMEM = {
    _OUT, _INV, _SET, _OFF_BTN, _NO, _YES, _MENU, _BLT, _SLP, _INF, _CUR, _LAN, _MNU, _TOGGLE,
    _OFF_NOW, _LEFT_ARROW, _RIGHT_ARROW, _PAGE_FWD, _PAGE_BCK, _PG_FWD, _PG_BCK, _MENU_TITLE,
    _SELECT, _LONG_PRESS, _CONFIRM, _INSUFF, _ONE_OR_MORE, _UNAVAIL, _PLEASE_WAIT, _REMAINING,
    _INVENTORY, _SETTINGS, _STATUS, _VERSION, _LCD_SET, _BACKLIGHT, _SLEEP, _LOCALE, _CURRENCY,
    _LANGUAGE, _DEVINFO, _HARDWARE, _FIRMWARE, _SOFTWARE, _PLATFORM, _FREEMEM, _UPTIME, _ERRS,
    _VALUES, _A, _B, _SYSTEM_OFF, _GOODBYE, _SHUTTING_DOWN, _LCD, _LOC, _VAL
};

#define WRITE_NONE { 0xFF, 0xFF, 0xFF, 0xFF }
#define WRITE_1(a) { a, 0xFF, 0xFF, 0xFF }
#define WRITE_2(a, b) { a, b, 0xFF, 0xFF }
#define WRITE_4(a, b, c, d) { a, b, c, d }

static const StackLabs::Locale::Row StackLabs::Locale::ROW_TABLE[SCREEN_COUNT * 3] PROGMEM = {
    // MENU/HOME (0):
    { S_MENU_TITLE, 8, WRITE_NONE },
    { 0xFF, 0, WRITE_NONE },
    { 0xFF, 0, WRITE_NONE },
    // OUT/DISPENSE (1):
    { S_SELECT_CHIPS, 0, WRITE_1(13) },
    { S_LONG_PRESS, 0, WRITE_NONE },
    { 0xFF, 0, WRITE_4(1,6,11,16)},
    // OUT/DISPENSE REVIEW (2):
    { S_CONFIRM_DISP, 0, WRITE_1(13) },
    { 0xFF, 0, WRITE_2(1, 11) },
    { 0xFF, 0, WRITE_2(1, 11) },
    // INSUFFICIENT STOCK (3):
    { S_INSUFF_STOCK, 1, WRITE_NONE },
    { S_ONE_OR_MORE, 1, WRITE_NONE },
    { S_CHIPS_UNAVAIL, 1, WRITE_NONE },
    // DISPENSING (4):
    { S_REMAINING, 1, WRITE_1(12) },
    { 0xFF, 0, WRITE_2(1, 11) },
    { 0xFF, 0, WRITE_2(1, 11) },
    // CHIP INVENTORY (5):
    { S_CHIP_INVENTORY, 3, WRITE_NONE },
    { 0xFF, 0, WRITE_2(1, 11) },
    { 0xFF, 0, WRITE_2(1, 11) },
    // SETTINGS (6):
    { S_SETTINGS, 6, WRITE_NONE },
    { S_STATUS, 1, WRITE_1(9) },
    { S_VERSION, 1, WRITE_1(5)  },
    // SETTINGS/LCD (7):
    { S_LCD_SETTINGS, 4, WRITE_NONE },
    { S_BACKLIGHT, 1, WRITE_1(11) },
    { S_SLEEP, 1, WRITE_1(8) },
    // SETTINGS/LOCALE (8):
    { S_LOCALE, 7, WRITE_NONE },
    { S_CURRENCY, 1, WRITE_1(10) },
    { S_LANGUAGE, 1, WRITE_1(10) },
    // SETTINGS/DEVICE INFO PG 1 (9):
    { S_DEVICE_INFO, 2, WRITE_1(17) },
    { S_HARDWARE, 0, WRITE_1(10) },
    { S_FIRMWARE, 0, WRITE_1(10) },
    // SETTINGS/DEVICE INFO PG 2 (10):
    { S_DEVICE_INFO, 2, WRITE_1(17) },
    { S_SOFTWARE, 0, WRITE_1(10) },
    { S_PLATFORM, 0, WRITE_1(10) },
    // SETTINGS/DEVICE INFO PG 3 (11):
    { S_DEVICE_INFO, 2, WRITE_1(17) },
    { S_FREEMEM, 0, WRITE_1(9) },
    { S_UPTIME, 0, WRITE_1(9) },
    // SETTINGS/DEVICE INFO PG 4 (12):
    { S_DEVICE_INFO, 2, WRITE_1(17) },
    { S_ERRS, 0, WRITE_1(6) },
    { S_ERRS, 0, WRITE_1(6) },
    // SETTINGS/VALUES (13):
    { S_VALUES, 4, WRITE_1(11) },
    { S_A, 1, WRITE_1(3) },
    { S_B, 1, WRITE_1(3) },
    // OFF (14):
    { S_SYSTEM_OFF, 5, WRITE_NONE },
    { S_GOODBYE, 6, WRITE_NONE },
    { S_SHUTTING_DOWN, 1, WRITE_1(16) },
};

#define BUTTON_(stringID, column, width) { stringID, column, width }
#define BUTTON_EMPTY { 0, 0, 0 }

static const StackLabs::Locale::Button StackLabs::Locale::BTN_TABLE[SCREEN_COUNT * 4] PROGMEM = {
    // MENU/HOME (0):
    BUTTON_(S_OUT, 0, 5),
    BUTTON_(S_INV, 5, 5),
    BUTTON_(S_SET, 10, 5),
    BUTTON_(S_OFF_BTN, 15, 5),
    // OUT/DISPENSE (1):
    BUTTON_EMPTY,
    BUTTON_EMPTY,
    BUTTON_EMPTY,
    BUTTON_EMPTY,
    // OUT/DISPENSE REVIEW (2):
    BUTTON_(S_NO, 0, 5),
    BUTTON_(S_MENU, 5, 10),
    BUTTON_(S_YES, 15, 5),
    BUTTON_EMPTY,
    // INSUFFICIENT STOCK (3):
    BUTTON_(S_MENU, 0, 20),
    BUTTON_EMPTY,
    BUTTON_EMPTY,
    BUTTON_EMPTY,
    // DISPENSING (4):
    BUTTON_(S_PLEASE_WAIT, 0, 20),
    BUTTON_EMPTY,
    BUTTON_EMPTY,
    BUTTON_EMPTY,
    // CHIP INVENTORY (5):
    BUTTON_(S_MENU, 0, 20),
    BUTTON_EMPTY,
    BUTTON_EMPTY,
    BUTTON_EMPTY,
    // SETTINGS (6):
    BUTTON_(S_LEFT_ARROW, 0, 5),
    BUTTON_(S_LCD, 5, 5),
    BUTTON_(S_LOC, 10, 5),
    BUTTON_(S_VAL, 15, 5),
    // SETTINGS/LCD (7):
    BUTTON_(S_MENU, 0, 10),
    BUTTON_(S_BLT, 10, 5),
    BUTTON_(S_SLP, 15, 5),
    BUTTON_EMPTY,
    // SETTINGS/LOCALE (8):
    BUTTON_(S_LEFT_ARROW, 0, 5),
    BUTTON_(S_INF, 5, 5),
    BUTTON_(S_CUR, 10, 5),
    BUTTON_(S_LAN, 15, 5),
    // SETTINGS/DEVICE INFO PG 1 (9):
    BUTTON_(S_LEFT_ARROW, 0, 5),
    BUTTON_(S_MNU, 5, 5),
    BUTTON_(S_PAGE_FWD, 10, 10),
    BUTTON_EMPTY,
    // SETTINGS/DEVICE INFO PG 2 (10):
    BUTTON_(S_LEFT_ARROW, 0, 5),
    BUTTON_(S_MNU, 5, 5),
    BUTTON_(S_PG_BCK, 10, 5),
    BUTTON_(S_PG_FWD, 15, 5),
    // SETTINGS/DEVICE INFO PG 3 (11):
    BUTTON_(S_LEFT_ARROW, 0, 5),
    BUTTON_(S_MNU, 5, 5),
    BUTTON_(S_PG_BCK, 10, 5),
    BUTTON_(S_PG_FWD, 15, 5),
    // SETTINGS/DEVICE INFO PG 4 (12):
    BUTTON_(S_LEFT_ARROW, 0, 5),
    BUTTON_(S_MNU, 5, 5),
    BUTTON_(S_PAGE_BCK, 10, 10),
    BUTTON_EMPTY,
    // SETTINGS/VALUES (13):
    BUTTON_(S_MENU, 0, 10),
    BUTTON_(S_TOGGLE, 10, 10),
    BUTTON_EMPTY,
    BUTTON_EMPTY,
    // OFF (14):
    BUTTON_(S_LEFT_ARROW, 0, 5),
    BUTTON_(S_OFF_NOW, 5, 15),
    BUTTON_EMPTY,
    BUTTON_EMPTY,
};
