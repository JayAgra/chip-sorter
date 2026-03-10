//
//  ColorSensor.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/10/26.
//

#include "ColorSensor.h"
#include "StackLabs.h"
#include <DFRobot_TCS34725.h>

const uint8_t COLOR_THRESHOLD = 50;
const uint8_t BLACK_THRESHOLD = 200;

namespace StackLabs {
    namespace ColorSensor {
        DFRobot_TCS34725 sensor;

        uint16_t red, green, blue, clear;

        void setup() {
            sensor.begin();
            sensor.setIntegrationtime(TCS34725_INTEGRATIONTIME_700MS);
            sensor.setGain(TCS34725_GAIN_16X); // need to test
        }

        void readColor() {
            sensor.getRGBC(&red, &green, &blue, &clear);
        }

        uint16_t getRed() {
            return red;
        }

        uint16_t getGreen() {
            return green;
        }

        uint16_t getBlue() {
            return blue;
        }

        uint16_t getClear() {
            return clear;
        }

        uint8_t calculateMatch() {
            if (clear < BLACK_THRESHOLD && red < COLOR_THRESHOLD &&
                green < COLOR_THRESHOLD && blue < COLOR_THRESHOLD) {
                return 0;
            }

            if (red > green + COLOR_THRESHOLD &&
                red > blue + COLOR_THRESHOLD) {
                return 1;
            } else if (green > red + COLOR_THRESHOLD &&
                green > blue + COLOR_THRESHOLD) {
                return 2;
            }
            return 3;
        }
    }
}