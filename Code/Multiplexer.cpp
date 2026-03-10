//
//  Multiplexer.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/10/26.
//

#include "StackLabs.h"
#include "Multiplexer.h"
#include <Arduino.h>

const int MUX_SIGNAL = A1;
const int MUX_CONTROL[] = {10, 11, 12, 13};

// 0 = free, 1 = buttons, 2 = motor 1, 3 = motor 2
uint8_t owner = 0;
uint8_t mode = INPUT;

namespace StackLabs {
    namespace Multiplexer {
        void setup() {
            for (size_t i = 0; i < 4; ++i) {
                pinMode(MUX_CONTROL[i], OUTPUT);
            }
            pinMode(MUX_SIGNAL, mode);
        }

        bool takeOwnership(uint8_t requester, uint8_t setMode) {
            if (owner == 0 || owner == requester) {
                owner = requester;
                mode = setMode;
                pinMode(MUX_SIGNAL, mode);
                return true;
            }
            return false;
        }

        bool releaseOwnership(uint8_t requester) {
            if (owner == requester) {
                owner = 0;
            }
        }

        bool selectChannel(uint8_t requester, uint8_t channel) {
            if (requester != owner) { return false; }
            if (channel > 15) { return false; }
            for (int i = 0; i < 4; ++i) {
                digitalWrite(MUX_CONTROL[(size_t) i], (channel >> i) & 1);
            }
            return true;
        }

        int8_t read(uint8_t requester) {
            if (owner == requester && mode == INPUT) {
                return digitalRead(MUX_SIGNAL) == HIGH;
            } else {
                return -1;
            }
        }

        bool write(uint8_t requester, uint8_t value) {
            if (requester != owner || mode == INPUT) { return false; }
            digitalWrite(MUX_SIGNAL, value);
            return true;
        }
    }
}