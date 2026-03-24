//
//  StackLabs.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/9/26.
//

#include "StackLabs.h"

namespace StackLabs {
    void setup() {
        #if DEBUG_FIRMWARE
        Serial.begin(9600);
        Serial << "CHIP SORTER BOOTED\nDEBUG MODE ON\nVERSION\n\tHW: "
               << HARDWARE << "\n\tFW: " << FIRMWARE << "\n\tSW: " << SOFTWARE
               << "\n";
        #endif
        LCD::setup();
        Multiplexer::setup();
        Stepper::setupBothSteppers();
        ColorSensor::setup();
    }
}