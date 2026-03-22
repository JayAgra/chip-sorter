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
}

void loop() {
    for (uint8_t i = 0; i < 15; i++) {
        LCD::printEmptyState(i);
        if (i == 1 || i == 2) {
            LCD::fillValue(i, 0, 0, "$123456");
        }
        delay(2000);
    }
}
