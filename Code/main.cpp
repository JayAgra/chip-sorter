//
//  main.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/8/26.
//

#include "StackLabs.h"

using namespace StackLabs;

uint8_t dataValue;

void setup() {
    StackLabs::setup();
    Serial.println("hello from main.cpp");
    Stepper::Stepper1.setSpeed(2000);
    Stepper::Stepper2.setSpeed(2000);
    dataValue = Data::read(0);
}

void loop() {
    Buttons::Press input = Buttons::getPress();
    Data::write(0, input.button, true);
    LCD::printEmptyState(input.button);
    LCD::fillValue(input.button, 2, 0, dataValue);
    Stepper::Stepper1.move(50 * input.button, 10);
    Stepper::Stepper2.move(50 * input.button, 10);
}
