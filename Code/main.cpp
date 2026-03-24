//
//  main.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/8/26.
//

#include "StackLabs.h"

using namespace StackLabs;

void setup() {
    StackLabs::setup();
    Serial.println("HELLO");
    Stepper::Stepper1.setSpeed(2000);
    Stepper::Stepper2.setSpeed(2000);
}

void loop() {
    Buttons::Press input = Buttons::getPress();
    LCD::printEmptyState(input.button);
    Stepper::Stepper1.move(50 * input.button, 10);
    Stepper::Stepper2.move(50 * input.button, 10);
}
