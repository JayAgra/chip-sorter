//
//  ColorSensor.h
//  SLDP
//
//  Created by Jayen Agrawal on 3/10/26.
//

#ifndef COLORSENSOR_H
#define COLORSENSOR_H

#include "StackLabs.h"
#include <DFRobot_TCS34725.h>

namespace StackLabs {
    namespace ColorSensor {
        extern DFRobot_TCS34725 sensor;

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
        uint16_t getRed();

        /**
         * @brief   Gets the most recent red value.
         * 
         * @return  Most recent red value.
        */
        uint16_t getGreen();

        /**
         * @brief   Gets the most recent blue value.
         * 
         * @return  Most recent blue value.
        */
        uint16_t getBlue();

        /**
         * @brief   Gets the most recent clear value.
         * 
         * @return  Most recent clear value.
        */
        uint16_t getClear();

        /**
         * @brief   Calculates the most likely poker chip being scanned.
         * 
         * @return  Integer representing the poker chip. 0 = Black, 1 = Red,
         *          2 = Green, 3 = Blue.
        */
        uint8_t calculateMatch();
    }
}

#endif
