//
//  Locale.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/9/26.
//

#include "Locale.h"

const char *LOCALES[2] = {
    "en-US",
    "es-MX",
};

const char *CURRENCIES[4] = {
    "USD",
    "EUR",
    "GBP",
    "JPY",
};

const char *TRANSLATIONS[2][59] = {
    {
        // MENU/HOME
        "        MENU",
        "OUT",
        "INV",
        "SET",
        "OFF",
        // OUT/DISPENSE
        "SELECT CHIPS %s",
        "LONG PRESS TO REVIEW",
        // OUT/DISPENSE REVIEW
        "CONFIRM DISP %s",
        "NO!",
        "  MENU  ",
        "YES",
        // INSUFFICIENT STOCK
        " INSUFFICIENT STOCK",
        " ONE OR MORE RQSTD.",
        " CHIPS UNAVAILABLE!",
        "       MENU       ",
        // DISPENSING
        " REMAINING: %s",
        "  PLEASE WAIT...  ",
        // CHIP INVENTORY
        "   CHIP INVENTORY",
        "       MENU       ",
        // SETTINGS
        "      SETTINGS",
        " STATUS: %s",
        " VER: H%sF%sS%s",
        "LCD",
        "LOC",
        "VAL",
        // SETTINGS/LCD
        "    LCD SETTINGS",
        " BACKLIGHT: %s",
        " SLEEP: |%03d| SECONDS ",
        "  MENU  ",
        "BLT",
        "SLP",
        // SETTINGS/LOCALE
        "       LOCALE",
        " CURRENCY: %s",
        " LANGUAGE: %s",
        "INF",
        "CUR",
        "LAN",
        // SETTINGS/DEVICE INFO
        "  DEVICE INFO PG %d",
        "HARDWARE: %s",
        "FIRMWARE: %s",
        "MNU",
        " PAGE %s ",
        "SOFTWARE: %s",
        "PLATFORM: %s",
        "%sPG",
        "PG%s",
        "FREEMEM: |%04d| BYTES",
        "UPTIME:  |%04d| MIN",
        "ERRS: %s",
        " %s PAGE ",
        // SETTINGS/VALUES
        "    VALUES (%s)",
        "%sA: %s %s %s %s",
        "%sB: %s %s %s %s",
        "  MENU  ",
        " TOGGLE ",
        // OFF
        "     SYSTEM OFF",
        "      GOODBYE!",
        " SHUTTING DOWN: |%02d|S",
        "   OFF NOW   "
    },
    {
        // MENU/HOME
        "        MENU",
        "OUT",
        "INV",
        "SET",
        "OFF",
        // OUT/DISPENSE
        "SELECT CHIPS %s",
        "LONG PRESS TO REVIEW",
        // OUT/DISPENSE REVIEW
        "CONFIRM DISP %s",
        "NO!",
        "  MENU  ",
        "YES",
        // INSUFFICIENT STOCK
        " INSUFFICIENT STOCK",
        " ONE OR MORE RQSTD.",
        " CHIPS UNAVAILABLE!",
        "       MENU       ",
        // DISPENSING
        " REMAINING: %s",
        "  PLEASE WAIT...  ",
        // CHIP INVENTORY
        "   CHIP INVENTORY",
        "       MENU       ",
        // SETTINGS
        "      SETTINGS",
        " STATUS: %s",
        " VER: H%sF%sS%s",
        "LCD",
        "LOC",
        "VAL",
        // SETTINGS/LCD
        "    LCD SETTINGS",
        " BACKLIGHT: %s",
        " SLEEP: |%03d| SECONDS ",
        "  MENU  ",
        "BLT",
        "SLP",
        // SETTINGS/LOCALE
        "       LOCALE",
        " CURRENCY: %s",
        " LANGUAGE: %s",
        "INF",
        "CUR",
        "LAN",
        // SETTINGS/DEVICE INFO
        "  DEVICE INFO PG %d",
        "HARDWARE: %s",
        "FIRMWARE: %s",
        "MNU",
        " PAGE %s ",
        "SOFTWARE: %s",
        "PLATFORM: %s",
        "%sPG",
        "PG%s",
        "FREEMEM: |%04d| BYTES",
        "UPTIME:  |%04d| MIN",
        "ERRS: %s",
        " %s PAGE ",
        // SETTINGS/VALUES
        "    VALUES (%s)",
        "%sA: %s %s %s %s",
        "%sB: %s %s %s %s",
        "  MENU  ",
        " TOGGLE ",
        // OFF
        "     SYSTEM OFF",
        "      GOODBYE!",
        " SHUTTING DOWN: |%02d|S",
        "   OFF NOW   "
    },
};

