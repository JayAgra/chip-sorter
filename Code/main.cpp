//
//  main.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/8/26.
//

#include "StackLabs.h"
#include <Arduino.h>
#include <avr/pgmspace.h>

void setup() {
    StackLabs::LCD::setup();
    StackLabs::LCD::createCharacters();
    StackLabs::ColorSensor::setup();
}

void loop() {
    for (uint8_t i = 0; i < 15; i++) {
        StackLabs::LCD::printEmptyState(i);
        if (i == 1 || i == 2) {
            StackLabs::LCD::fillValue(i, 0, 0, "$123456");
        }
        delay(2000);
    }
}
