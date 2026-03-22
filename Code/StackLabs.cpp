//
//  StackLabs.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/9/26.
//

#include "StackLabs.h"

namespace StackLabs {
    void setup() {
        LCD::setup();
        Multiplexer::setup();
        Stepper::setupBothSteppers();
        ColorSensor::setup();
    }
}