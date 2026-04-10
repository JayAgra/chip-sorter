//
//  ColorSensor.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/10/26.
//

#include "ColorSensor.h"

static const uint8_t COLOR_THRESHOLD = 15;
static const uint8_t BLACK_THRESHOLD = 25;
static const uint8_t BLACK_MAXIMUM = 100;

static const uint16_t RED_WHITE = 15;
static const uint16_t RED_BLACK = 927;
static const uint16_t GREEN_WHITE = 22;
static const uint16_t GREEN_BLACK = 950;
static const uint16_t BLUE_WHITE = 130;
static const uint16_t BLUE_BLACK = 800;
static const uint16_t CLEAR_WHITE = 21;
static const uint16_t CLEAR_BLACK = 750;

static Adafruit_TCS34725 sensor = 
    Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_1X);

namespace StackLabs {
    namespace ColorSensor {
        uint16_t red, green, blue, clear;

        void setup() {
            while (!sensor.begin()) {
                // write an error or something idk
                #if DEBUG_FIRMWARE
                Serial << "Color sensor init failed" << "\n";
                #endif
                delay(500);
            }
        }

        void readColor() {
            sensor.getRawData(&red, &green, &blue, &clear);
            #if DEBUG_FIRMWARE
            Serial << "Color sensor read: \n\tR: " << red << "\n\tG: " << green
                   << "\n\tB: " << blue << "\n\tC: " << clear << "\n";
             #endif
        }

        static uint8_t getRed() {
            return uint8_t(map(min(max(red, RED_WHITE), RED_BLACK),
                    RED_WHITE, RED_BLACK, 0, 255));
        }

        static uint8_t getGreen() {
            return uint8_t(map(min(max(green, GREEN_WHITE), GREEN_BLACK),
                    GREEN_WHITE, GREEN_BLACK, 0, 255));
        }

        static uint8_t getBlue() {
            return uint8_t(map(min(max(blue, BLUE_WHITE), BLUE_BLACK),
                    BLUE_WHITE, BLUE_BLACK, 0, 255));
        }

        static uint8_t getClear() {
            return uint8_t(map(min(max(clear, CLEAR_WHITE), CLEAR_BLACK),
                    CLEAR_WHITE, CLEAR_BLACK, 0, 255));
        }

        uint8_t calculateMatch() {
            if (getRed() > getGreen() + COLOR_THRESHOLD &&
                getRed() > getBlue() + COLOR_THRESHOLD) {
                return 0;
            } else if (getGreen() > getRed() + COLOR_THRESHOLD &&
                getGreen() > getBlue() + COLOR_THRESHOLD) {
                return 1;
            } else if (getBlue() > getRed() + COLOR_THRESHOLD &&
                getBlue() > getGreen() + COLOR_THRESHOLD) {
                return 2;
            } else if (getClear() > BLACK_THRESHOLD &&
                getClear() < BLACK_MAXIMUM) {
                return 3;
            }
            return 4;
        }
    }
}