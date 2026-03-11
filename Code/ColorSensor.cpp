//
//  ColorSensor.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/10/26.
//

#include "ColorSensor.h"
#include "StackLabs.h"
#include <Adafruit_TCS34725.h>
#include <HardwareSerial.h>

const uint8_t COLOR_THRESHOLD = 15;
const uint8_t BLACK_THRESHOLD = 25;
const uint8_t BLACK_MAXIMUM = 100;

Adafruit_TCS34725 sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_1X);

const uint16_t RED_WHITE = 2200;
const uint16_t RED_BLACK = 100;
const uint16_t GREEN_WHITE = 1800;
const uint16_t GREEN_BLACK = 75;
const uint16_t BLUE_WHITE = 1300;
const uint16_t BLUE_BLACK = 40;
const uint16_t CLEAR_WHITE = 6000;
const uint16_t CLEAR_BLACK = 150;

namespace StackLabs {
    namespace ColorSensor {
        uint16_t red, green, blue, clear;

        void setup() {
            while (!sensor.begin()) {
                Serial.println("No Sensor Found.");
                delay(1000);
            }
        }

        void readColor() {
            sensor.getRawData(&red, &green, &blue, &clear);
        }

        uint16_t getRed() {
            return uint16_t(map(max(min(red, RED_WHITE), RED_BLACK), RED_BLACK, RED_WHITE, 0, 255));
        }

        uint16_t getGreen() {
            return uint16_t(map(max(min(green, GREEN_WHITE), GREEN_BLACK), GREEN_BLACK, GREEN_WHITE, 0, 255));
        }

        uint16_t getBlue() {
            return uint16_t(map(max(min(blue, BLUE_WHITE), BLUE_BLACK), BLUE_BLACK, BLUE_WHITE, 0, 255));
        }

        uint16_t getClear() {
            return uint16_t(map(max(min(clear, CLEAR_WHITE), CLEAR_BLACK), CLEAR_BLACK, CLEAR_WHITE, 0, 255));
        }

        uint8_t calculateMatch() {
            if (getRed() > getGreen() + COLOR_THRESHOLD &&
                getRed() > getBlue() + COLOR_THRESHOLD) {
                return 2;
            } else if (getGreen() > getRed() + COLOR_THRESHOLD &&
                getGreen() > getBlue() + COLOR_THRESHOLD) {
                return 3;
            } else if (getBlue() > getRed() + COLOR_THRESHOLD &&
                getBlue() > getGreen() + COLOR_THRESHOLD) {
                return 4;
            } else if (getClear() > BLACK_THRESHOLD && getClear() < BLACK_MAXIMUM) {
                return 1;
            }
            return 0;
        }
    }
}