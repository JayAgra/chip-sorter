//
//  Main.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 4/6/26.
//

#include "StackLabs.h"

using namespace StackLabs;

#define TOP_HOME_OFFSET     0
#define BOT_HOME_OFFSET     0

// steps from feeder to non-overshot pos
#define TOP_STEPS_RED       54
#define TOP_STEPS_GREEN     87
#define TOP_STEPS_BLUE      120
#define TOP_STEPS_BLACK     -100

// overshoot for the fancy weird thing
#define TOP_OVERSHOOT       30

// steps from aligned with output hole
#define BOT_STEPS_RED       74 // 54
#define BOT_STEPS_GREEN     107
#define BOT_STEPS_BLUE      -102
#define BOT_STEPS_BLACK     -70 // -50

// Motor speeds (lower is faster)
#define TOP_SPEED           0xFF
#define BOT_SPEED           0x7F
#define TOP_ACCEL           10
#define BOT_ACCEL           10

// data handled by my magic script
#define EEPROM_COUNT_RED    0
#define EEPROM_COUNT_GREEN  1
#define EEPROM_COUNT_BLUE   2
#define EEPROM_COUNT_BLACK  3
#define EEPROM_CURRENCY     4
#define EEPROM_VALUE_SET    5
#define EEPROM_RESET_NEXT   9

// state variables
static uint8_t chipCounts[4];
static uint8_t dispenseRequest[4];
static uint8_t currencyID;
static uint8_t valueSetID;
static uint8_t uiState;

static void homeTopDisc() {
    Stepper::Stepper1.home();
    // Stepper::Stepper1.setDirection(MotorDirection::CLOCKWISE);
    // ColorSensor::readColor();
    // if (ColorSensor::calculateMatch() != 4) {
    //     Stepper::Stepper1.move(100, TOP_ACCEL);
    //     ColorSensor::readColor();
    // }
    // while (ColorSensor::calculateMatch() == 4) {
    //     Stepper::Stepper1.move(10, TOP_ACCEL);
    //     ColorSensor::readColor();
    // }
    // Stepper::Stepper1.move(35, TOP_ACCEL);
    // ColorSensor::readColor();
    // if (ColorSensor::calculateMatch() != 4) {
    //     homeTopDisc();
    // }
}

static void homeBotDisc() {
    Stepper::Stepper2.home();
}

// SETUP
void setup() {
    if (Data::readOr(EEPROM_RESET_NEXT, 0) == 1) {
        Stepper::Stepper1.setZero();
        Stepper::Stepper2.setZero();
        Data::write(EEPROM_RESET_NEXT, 0, true);
    }

    StackLabs::setup();

    chipCounts[0] = Data::readOr(EEPROM_COUNT_RED, 0);
    chipCounts[1] = Data::readOr(EEPROM_COUNT_GREEN, 0);
    chipCounts[2] = Data::readOr(EEPROM_COUNT_BLUE, 0);
    chipCounts[3] = Data::readOr(EEPROM_COUNT_BLACK, 0);
    currencyID = Data::readOr(EEPROM_CURRENCY, 0);
    valueSetID = Data::readOr(EEPROM_VALUE_SET, 0);

    Stepper::Stepper1.setSpeed(TOP_SPEED);
    Stepper::Stepper2.setSpeed(BOT_SPEED);

    Stepper::Stepper1.setDirection(MotorDirection::CLOCKWISE);
    Stepper::Stepper2.setDirection(MotorDirection::CLOCKWISE);

    homeTopDisc();
    homeBotDisc();

    uiState = 0;
}

static uint32_t computeTotal(const uint8_t counts[4]) {
    uint32_t total = 0;
    for (uint8_t i = 0; i < 4; ++i) {
        total += (uint32_t)counts[i] * get_chip_value(currencyID, valueSetID, i);
    }
    return total;
}

// Formats a 7-char total field: symbol + 6 digits (or JPY K-format).
// buf must be at least 8 bytes.
static void formatTotal(uint32_t total, char *buf) {
    char sym = get_currency_symbol(currencyID);
    if (currencyID == 3) {
        // JPY: "¥000.0K"
        uint32_t k    = total / 100;
        uint32_t frac = (total % 100) / 10;
        buf[0] = sym;
        buf[1] = '0' + (char)((k / 100) % 10);
        buf[2] = '0' + (char)((k / 10)  % 10);
        buf[3] = '0' + (char)(k % 10);
        buf[4] = '.';
        buf[5] = '0' + (char)(frac);
        buf[6] = 'K';
        buf[7] = '\0';
    } else {
        buf[0] = sym;
        buf[1] = '0' + ((total / 100000) % 10);
        buf[2] = '0' + ((total / 10000)  % 10);
        buf[3] = '0' + ((total / 1000)   % 10);
        buf[4] = '0' + ((total / 100)    % 10);
        buf[5] = '0' + ((total / 10)     % 10);
        buf[6] = '0' + (total            % 10);
        buf[7] = '\0';
    }
}

static void formatCount2(uint8_t n, char *buf) {
    buf[0] = '0' + (n / 10) % 10;
    buf[1] = '0' + n % 10;
    buf[2] = '\0';
}

static void formatCount3(uint8_t n, char *buf) {
    buf[0] = '0' + (n / 100) % 10;
    buf[1] = '0' + (n / 10)  % 10;
    buf[2] = '0' + n % 10;
    buf[3] = '\0';
}

// DENOMINATION LABEL FORMATTING
// Produces "SYM VAL: NN" for a review/inventory row slot.
// e.g. "$  5: 07", "¥ 1K: 03"
static void formatDenomLabel(uint8_t tube, uint8_t count, char *buf) {
    char sym = get_currency_symbol(currencyID);
    uint16_t val = get_chip_value(currencyID, valueSetID, tube);
    char valStr[4];

    if (currencyID == 3) {
        if (val >= 1000) {
            uint32_t k = val / 1000;
            if (k >= 10) {
                valStr[0] = '0' + (char)((k / 10) % 10);
                valStr[1] = '0' + (char)(k % 10);
                valStr[2] = 'K';
            } else {
                valStr[0] = ' ';
                valStr[1] = '0' + (char)(k % 10);
                valStr[2] = 'K';
            }
        } else {
            valStr[0] = '0' + (char)((val / 100) % 10);
            valStr[1] = '0' + (char)((val / 10)  % 10);
            valStr[2] = '0' + (char)(val % 10);
        }
    } else {
        if (val >= 100) {
            valStr[0] = '0' + (char)((val / 100) % 10);
            valStr[1] = '0' + (char)((val / 10)  % 10);
            valStr[2] = '0' + (char)(val % 10);
        } else if (val >= 10) {
            valStr[0] = ' ';
            valStr[1] = '0' + (char)((val / 10) % 10);
            valStr[2] = '0' + (char)(val % 10);
        } else {
            valStr[0] = ' ';
            valStr[1] = ' ';
            valStr[2] = '0' + (char)(val % 10);
        }
    }
    valStr[3] = '\0';

    char cntBuf[3];
    formatCount2(count, cntBuf);

    // sym + 3-char value + ": " + 2-char count = 8 chars
    buf[0] = sym;
    buf[1] = valStr[0];
    buf[2] = valStr[1];
    buf[3] = valStr[2];
    buf[4] = ':';
    buf[5] = ' ';
    buf[6] = cntBuf[0];
    buf[7] = cntBuf[1];
    buf[8] = '\0';
}

// DISPENSE BUTTON ROW (state 1 row 3)
static void writeDispenseButtonRow() {
    char sym = get_currency_symbol(currencyID);

    for (uint8_t i = 0; i < 4; ++i) {
        uint16_t val = get_chip_value(currencyID, valueSetID, i);
        char valStr[4];

        if (currencyID == 3) {
            if (val >= 1000) {
                uint32_t k = val / 1000;
                if (k >= 10) {
                    valStr[0] = '0' + (char)((k / 10) % 10);
                    valStr[1] = '0' + (char)(k % 10);
                    valStr[2] = 'K';
                } else {
                    valStr[0] = ' ';
                    valStr[1] = '0' + (char)(k % 10);
                    valStr[2] = 'K';
                }
            } else {
                valStr[0] = '0' + (char)((val / 100) % 10);
                valStr[1] = '0' + (char)((val / 10)  % 10);
                valStr[2] = '0' + (char)(val % 10);
            }
        } else {
            if (val >= 100) {
                valStr[0] = '0' + (char)((val / 100) % 10);
                valStr[1] = '0' + (char)((val / 10)  % 10);
            } else if (val >= 10) {
                valStr[0] = ' ';
                valStr[1] = '0' + (char)((val / 10) % 10);
            } else {
                valStr[0] = ' ';
                valStr[1] = ' ';
            }
            valStr[2] = '0' + (char)(val % 10);
        }
        valStr[3] = '\0';

        LCD::fillButtonValue(valStr, i);
    }
}

// HOMING
static const int16_t TOP_TUBE_STEPS[4] = {
    TOP_STEPS_RED, TOP_STEPS_GREEN, TOP_STEPS_BLUE, TOP_STEPS_BLACK
};
static const int8_t BOT_TUBE_STEPS[4] = {
    BOT_STEPS_RED, BOT_STEPS_GREEN, BOT_STEPS_BLUE, BOT_STEPS_BLACK
};

// SORT SEQUENCE (top disc)
static uint8_t sortOneChip() {
    ColorSensor::readColor();
    uint8_t color = ColorSensor::calculateMatch();

    if (color == 4) return 0xFF;

    lcd.setCursor(0,0);
    lcd.print(color);
    
    int16_t tubeSteps = TOP_TUBE_STEPS[color];

    Stepper::Stepper1.vectorMove(tubeSteps, TOP_ACCEL);
    Stepper::Stepper1.vectorMove((tubeSteps > 0) ? TOP_OVERSHOOT :
        0, TOP_ACCEL);
    Stepper::Stepper1.vectorMove((tubeSteps > 0) ? -1 * TOP_OVERSHOOT :
        0, TOP_ACCEL);
    Stepper::Stepper1.vectorMove(-1 * tubeSteps, TOP_ACCEL);

    return color;
}

// DISPENSE SEQUENCE (bottom disc)
static void dispenseOneChip(uint8_t tube) {
    Stepper::Stepper2.vectorMove(BOT_TUBE_STEPS[tube], BOT_ACCEL);
    // Stepper::Stepper2.vectorMove(-1 * BOT_TUBE_STEPS[tube], BOT_ACCEL);
    if (BOT_TUBE_STEPS[tube] > 0) {
        Stepper::Stepper2.home();
    } else {
        // please do not read :)
        Stepper::Stepper2.setDirection(MotorDirection::CLOCKWISE);
        Stepper::Stepper2.move(abs(BOT_TUBE_STEPS[tube]), BOT_ACCEL / 2);
        Stepper::Stepper2.setZero();
    }
}

// SORTING MODE (triggered from inventory screen long press)
static void runSortingMode() {
    uint8_t noChipStreak = 0;
    const uint8_t MAX_NO_CHIP = 5;

    while (noChipStreak < MAX_NO_CHIP) {
        ColorSensor::readColor();
        uint8_t color = ColorSensor::calculateMatch();

        if (color == 4) {
            ++noChipStreak;
            delay(500);
            continue;
        }

        lcd.setCursor(0,0);
        lcd.write(color);
        
        int16_t tubeSteps = TOP_TUBE_STEPS[color];
        int16_t totalSteps = tubeSteps + TOP_OVERSHOOT;

        Stepper::Stepper1.vectorMove(tubeSteps, TOP_ACCEL);
        Stepper::Stepper1.vectorMove((tubeSteps > 0) ? TOP_OVERSHOOT :
            0, TOP_ACCEL);
        delay(50);
        Stepper::Stepper1.vectorMove((tubeSteps > 0) ? -1 * TOP_OVERSHOOT :
            0, TOP_ACCEL);
        delay(100);
        Stepper::Stepper1.vectorMove(-1 * tubeSteps, TOP_ACCEL);

        noChipStreak = 0;
        if (chipCounts[color] < 0xFF) ++chipCounts[color];

        Data::write(EEPROM_COUNT_RED + color, chipCounts[color], true);
    }
}

// SHARED CHIP ROW RENDERER
// Fills rows 1 & 2 of a given state with denom labels + counts.
// Used by states 2, 4, and 5.
static void fillChipRows(uint8_t state, const uint8_t counts[4]) {
    char buf[9];
    for (uint8_t r = 0; r < 2; ++r) {
        formatDenomLabel(r * 2,     counts[r * 2],     buf);
        LCD::fillValue(state, r + 1, 0, buf);
        formatDenomLabel(r * 2 + 1, counts[r * 2 + 1], buf);
        LCD::fillValue(state, r + 1, 1, buf);
    }
}

// STATE 0: MENU
static void handleMenu() {
    LCD::printEmptyState(0);

    Buttons::Press p = Buttons::getPress();
    switch (p.button) {
        case 0:
            uiState = 1;
            break;
        case 1:
            uiState = 5;
            break;
        case 2:
            uiState = 6;
            break;
        case 3:
            uiState = 14;
            break;
        default:
            break;
    }
}

// STATE 1: OUT/DISPENSE
static void handleDispense() {
    dispenseRequest[0] = 0;
    dispenseRequest[1] = 0;
    dispenseRequest[2] = 0;
    dispenseRequest[3] = 0;

    LCD::printEmptyState(1);
    writeDispenseButtonRow();

    auto renderDispense = [&]() {
        char totalBuf[8];
        formatTotal(computeTotal(dispenseRequest), totalBuf);
        LCD::fillValue(1, 0, 0, totalBuf);

        char cntBuf[4];
        for (uint8_t i = 0; i < 4; ++i) {
            formatCount3(dispenseRequest[i], cntBuf);
            LCD::fillValue(1, 2, i, cntBuf);
        }
    };

    renderDispense();

    while (true) {
        Buttons::Press p = Buttons::getPress();

        if (p.held) {
            if (
                dispenseRequest[0] == 0 &&
                memcmp(dispenseRequest, dispenseRequest + 1, 3) == 0
            ) {
                // no chips selected, exit to menu
                uiState = 0;
            } else if (
                // enough chips in storage for this order, continue
                dispenseRequest[0] <= chipCounts[0] &&
                dispenseRequest[1] <= chipCounts[1] &&
                dispenseRequest[2] <= chipCounts[2] &&
                dispenseRequest[3] <= chipCounts[3]
            ) {
                uiState = 2;
            } else {
                // not enough chips
                uiState = 3;
            }
            return;
        }

        uint8_t tube = p.button;
        if (dispenseRequest[tube] < 101) {
            ++dispenseRequest[tube];
        }

        renderDispense();

        delay(100);
    }
}

// STATE 2: OUT/DISPENSE REVIEW
static void handleReview() {
    LCD::printEmptyState(2);

    char totalBuf[8];
    formatTotal(computeTotal(dispenseRequest), totalBuf);
    LCD::fillValue(2, 0, 0, totalBuf);

    fillChipRows(2, dispenseRequest);

    Buttons::Press p = Buttons::getPress();
    switch (p.button) {
        case 0:
            uiState = 1;
            break;
        case 1:
        case 2:
            uiState = 0;
            break;
        case 3:
        {
            bool ok = true;
            for (uint8_t i = 0; i < 4; ++i) {
                if (dispenseRequest[i] > chipCounts[i]) { ok = false; break; }
            }
            uiState = ok ? 4 : 3;
            break;
        }
        default: break;
    }
}

// STATE 3: INSUFFICIENT STOCK
static void handleInsufficient() {
    LCD::printEmptyState(3);
    Buttons::Press p = Buttons::getPress();
    (void)p;
    uiState = 0;
}

// STATE 4: DISPENSING
static void handleDispensing() {
    uint8_t remaining[4];
    for (uint8_t i = 0; i < 4; ++i) remaining[i] = dispenseRequest[i];

    for (uint8_t tube = 0; tube < 4; ++tube) {
        while (remaining[tube] > 0) {
            LCD::printEmptyState(4);

            char totalBuf[8];
            formatTotal(computeTotal(remaining), totalBuf);
            LCD::fillValue(4, 0, 0, totalBuf);

            fillChipRows(4, remaining);

            dispenseOneChip(tube);

            --remaining[tube];
            if (chipCounts[tube] > 0) --chipCounts[tube];
            Data::write(EEPROM_COUNT_RED + tube, chipCounts[tube], true);
        }
    }

    uiState = 0;
}

// STATE 5: CHIP INVENTORY
static void handleInventory() {
    LCD::printEmptyState(5);
    fillChipRows(5, chipCounts);

    Buttons::Press p = Buttons::getPress();
    uiState = 0;
}

// STATE 6: SETTINGS
static void handleSettings() {
    LCD::printEmptyState(6);
    LCD::fillValue(6, 1, 0, (const char *)"OK");

    char verBuf[13];
    verBuf[0]  = 'H'; verBuf[1]  = '1'; verBuf[2]  = '0'; verBuf[3]  = '0';
    verBuf[4]  = 'F'; verBuf[5]  = '1'; verBuf[6]  = '0'; verBuf[7]  = '0';
    verBuf[8]  = 'S'; verBuf[9]  = '1'; verBuf[10] = '0'; verBuf[11] = '0';
    verBuf[12] = '\0';
    LCD::fillValue(6, 2, 0, verBuf);

    Buttons::Press p = Buttons::getPress();
    switch (p.button) {
        case 0:
            uiState = 0;
            break;
        case 1:
            uiState = 7;
            break;
        case 2:
            uiState = 8;
            break;
        case 3:
            uiState = 13;
            break;
        default: break;
    }
}

// STATE 7: CHIP INPUT
static void handleChipInput() {
    LCD::printEmptyState(7);

    Buttons::Press p = Buttons::getPress();
    switch (p.button) {
        case 0:
        default:
            uiState = 6;
            break;
        case 1:
        case 2:
            // for (uint8_t i = 0; i < 10;) {
            //     if (sortOneChip() == 0xFF) {
            //         ++i;
            //     }
            //     delay(100);
            // }
            // delay(1000);
            LCD::printEmptyState(15);
            runSortingMode();
            LCD::printEmptyState(7);
            uiState = 6;
            break;
        case 3:
            Data::reset();


            if (p.held) {
                Stepper::Stepper1.setZero();
                Stepper::Stepper2.setZero();
                Data::write(EEPROM_RESET_NEXT, 1, true);
            }

            Stepper::Stepper1.setSleep(true);
            Stepper::Stepper2.setSleep(true);

            LCD::clear();

            break;
    }
}

// STATE 8: SETTINGS/LOCALE
static void handleLocale() {
    LCD::printEmptyState(8);
    // "USD ($)"
    const char* currencyCode = get_currency_code(currencyID);
    char curBuf[8];
    curBuf[0] = currencyCode[0];
    curBuf[1] = currencyCode[1];
    curBuf[2] = currencyCode[2];
    curBuf[3] = ' ';
    curBuf[4] = '(';
    curBuf[5] = get_currency_symbol(currencyID);
    curBuf[6] = ')';
    curBuf[7] = '\0';
    LCD::fillValue(8, 1, 0, curBuf);

    LCD::fillValue(8, 2, 0, (const char *)"EN-US");

    Buttons::Press p = Buttons::getPress();
    switch (p.button) {
        case 0:
            uiState = 6;
            break;
        case 1:
            uiState = 9;
            break;
        case 2:
            currencyID = (currencyID + 1) % 5;
            Data::write(EEPROM_CURRENCY, currencyID, true);
            break;
        case 3: break; // do nothing because other lang support not fixed
        default: break;
    }
}

// STATES 9–12: DEVICE INFO PAGES
static void handleInfo(uint8_t page) {
    uint8_t state = 9 + page;
    LCD::printEmptyState(state);

    if (page == 0) {
        LCD::fillValue(9, 1, 0, HARDWARE);
        LCD::fillValue(9, 2, 0, SOFTWARE);
    } else if (page == 1) {
        LCD::fillValue(10, 1, 0, FIRMWARE);
        LCD::fillValue(10, 2, 0, PLATFORM);
    } else if (page == 2) {
        extern int __heap_start, *__brkval;
        int freeRam = (int)&freeRam -
            (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
        char memBuf[6];
        memBuf[0] = '0' + (freeRam / 10000) % 10;
        memBuf[1] = '0' + (freeRam / 1000)  % 10;
        memBuf[2] = '0' + (freeRam / 100)   % 10;
        memBuf[3] = '0' + (freeRam / 10)    % 10;
        memBuf[4] = '0' + freeRam % 10;
        memBuf[5] = '\0';
        LCD::fillValue(11, 1, 0, memBuf);

        uint32_t uptimeMins = millis() / 60000UL;
        char uptBuf[5];
        uptBuf[0] = '0' + (char)((uptimeMins / 1000) % 10);
        uptBuf[1] = '0' + (char)((uptimeMins / 100) % 10);
        uptBuf[2] = '0' + (char)((uptimeMins / 10) % 10);
        uptBuf[3] = '0' + (char)(uptimeMins % 10);
        uptBuf[4] = '\0';
        LCD::fillValue(11, 2, 0, uptBuf);
    } else {
        // LCD::fillValue(12, 1, 0, (const char *)"---- ---- ----");
        // LCD::fillValue(12, 2, 0, (const char *)"---- ---- ----");
    }

    Buttons::Press p = Buttons::getPress();
    switch (p.button) {
        case 0:
            uiState = 6;
            break;
        case 1:
            uiState = 0;
            break;
        case 2:
            uiState = (page == 0) ? 10 : (8 + page);
            break;
        case 3:
            uiState = (page < 3) ? (10 + page) : 9;
            break;
        default: break;
    }
}

// STATE 13: SETTINGS/VALUES
static void handleValues() {
    LCD::printEmptyState(13);
    const char* currencyCode = get_currency_code(currencyID);
    // Row 0 slot 0: "(USD)"
    char curBuf[6];
    curBuf[0] = '(';
    curBuf[1] = currencyCode[0];
    curBuf[2] = currencyCode[1];
    curBuf[3] = currencyCode[2];
    curBuf[4] = ')';
    curBuf[5] = '\0';
    LCD::fillValue(13, 0, 0, curBuf);

    for (uint8_t s = 0; s < 2; ++s) {
        LCD::fillValue(13, s + 1, 0, (s == valueSetID ? "*" : "\0"));

        // Value string: "005 010 025 100"
        char rowBuf[16];
        for (uint8_t t = 0; t < 4; ++t) {
            uint16_t v = get_chip_value(currencyID, s, t);
            uint8_t base = t * 4;

            if (currencyID == 3) {
                if (v >= 1000) {
                    uint32_t k = v / 1000;
                    rowBuf[base + 0] = '0' + (char)((k / 10) % 10);
                    rowBuf[base + 1] = '0' + (char)(k % 10);
                    rowBuf[base + 2] = 'K';
                } else {
                    rowBuf[base + 0] = '0' + (char)((v / 100) % 10);
                    rowBuf[base + 1] = '0' + (char)((v / 10) % 10);
                    rowBuf[base + 2] = '0' + (char)(v % 10);
                }
            } else {
                rowBuf[base + 0] = '0' + (char)((v / 100) % 10);
                rowBuf[base + 1] = '0' + (char)((v / 10) % 10);
                rowBuf[base + 2] = '0' + (char)(v % 10);
            }
            rowBuf[base + 3] = (t < 3) ? ' ' : '\0';
        }
        rowBuf[15] = '\0';
        LCD::fillValue(13, s + 1, 1, rowBuf);
    }

    Buttons::Press p = Buttons::getPress();
    switch (p.button) {
        case 0:
            uiState = 6;
            break;
        case 1:
            uiState = 0;
            break;
        case 2:
        case 3:
            valueSetID = (valueSetID + 1) % 2;
            Data::write(EEPROM_VALUE_SET, valueSetID, true);
            break;
        default: uiState = 0; break;
    }
}

//  STATE 14: OFF
//  More like a sleep mode.
static void handleOff() {
    LCD::printEmptyState(14);

    // sleep
    Stepper::Stepper1.setSleep(true);
    Stepper::Stepper2.setSleep(true);
    LCD::clear();
    LCD::backlight(false);

    // wake
    Buttons::Press p = Buttons::getPress();
    Stepper::Stepper1.setSleep(false);
    Stepper::Stepper2.setSleep(false);
    LCD::backlight(true);
    LCD::setup();
    uiState = 0;
}

// Do not change UI state to 15. Instead, call printEmptyState(15) as needed.
static void handleWait() {
    LCD::printEmptyState(15);
    delay(1000);
    uiState = 0;
    return;
}

// LOOP
void loop() {
    switch (uiState) {
        case 0:  handleMenu();          break;
        case 1:  handleDispense();      break;
        case 2:  handleReview();        break;
        case 3:  handleInsufficient();  break;
        case 4:  handleDispensing();    break;
        case 5:  handleInventory();     break;
        case 6:  handleSettings();      break;
        case 7:  handleChipInput();     break;
        case 8:  handleLocale();        break;
        case 9:  handleInfo(0);      break;
        case 10: handleInfo(1);      break;
        case 11: handleInfo(2);      break;
        case 12: handleInfo(3);      break;
        case 13: handleValues();        break;
        case 14: handleOff();           break;
        case 15: handleWait();          break;
        default: uiState = 0;           break;
    }
}