//
//  ColorSensor.h
//  SLDP
//
//  Created by Jayen Agrawal on 3/10/26.
//

#ifndef COLORSENSOR_H
#define COLORSENSOR_H

#include "StackLabs.h"

#include <Adafruit_TCS34725.h>

namespace StackLabs {
    namespace ColorSensor {
        /**
         * @brief   Sets up the color sensor.
        */
        void setup();

        /**
         * @brief   Reads color from the sensor.
        */
        void readColor();

        /**
         * @brief   Gets the most recent green value.
         * 
         * @return  Most recent green value.
        */
        uint8_t getRed();

        /**
         * @brief   Gets the most recent red value.
         * 
         * @return  Most recent red value.
        */
        uint8_t getGreen();

        /**
         * @brief   Gets the most recent blue value.
         * 
         * @return  Most recent blue value.
        */
        uint8_t getBlue();

        /**
         * @brief   Gets the most recent clear value.
         * 
         * @return  Most recent clear value.
        */
        uint8_t getClear();

        /**
         * @brief   Calculates the most likely poker chip being scanned.
         * 
         * @return  Integer representing the poker chip. 0 = None, 1 = Black,
         *          2 = Red, 3 = Green, 4 = Blue.
        */
        uint8_t calculateMatch();
    }
}

#endif
