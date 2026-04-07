//
//  ColorSensor.h
//  SLDP
//
//  Created by Jayen Agrawal on 3/10/26.
//

#ifndef COLORSENSOR_H
#define COLORSENSOR_H

#include "Environment.h"

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
         * @brief   Gets the most recent green value. Internal use only.
         * 
         * @return  Most recent green value.
        */
        static uint8_t getRed();

        /**
         * @brief   Gets the most recent red value. Internal use only.
         * 
         * @return  Most recent red value.
        */
        static uint8_t getGreen();

        /**
         * @brief   Gets the most recent blue value. Internal use only.
         * 
         * @return  Most recent blue value.
        */
        static uint8_t getBlue();

        /**
         * @brief   Gets the most recent clear value. Internal use only.
         * 
         * @return  Most recent clear value.
        */
        static uint8_t getClear();

        /**
         * @brief   Calculates the most likely poker chip being scanned.
         * 
         * @return  Integer representing the poker chip. 0 = Red, 1 = Green,
         *          2 = Blue, 3 = Black, 4 = None.
        */
        uint8_t calculateMatch();
    }
}

#endif
