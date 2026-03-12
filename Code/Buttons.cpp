//
//  Buttons.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/10/26.
//

#include "StackLabs.h"
#include "Buttons.h"

static const uint8_t MUX_REQUESTER_ID = 1;
static unsigned long startTimes[4];

namespace StackLabs {
    namespace Buttons {
        void setup() {
            Multiplexer::setup();
        }

        Press getPress() {
            bool muxOwnership = false;
            // wait until we own the multiplexer
            while (!muxOwnership) {
                muxOwnership =
                    Multiplexer::takeOwnership(MUX_REQUESTER_ID, INPUT_PULLUP);
                delay(500);
            }
            while (true) {
                for (int i = 0; i < 4; ++i) {
                    Multiplexer::selectChannel(MUX_REQUESTER_ID, i);

                    int8_t buttonValue = Multiplexer::read(MUX_REQUESTER_ID);

                    switch (buttonValue) {
                    case 0:
                        if (startTimes[i] == 0) {
                            startTimes[i] = millis();
                        }
                        break;
                    case 1:
                        if (startTimes[i] == 0) {
                            unsigned long pressLength =
                                millis() - startTimes[i];
                            // try to ignore noise
                            if (pressLength > 20) {
                                Multiplexer::releaseOwnership(MUX_REQUESTER_ID);
                                Press tmp = Press();
                                tmp.button = i;
                                tmp.held = pressLength > LONG_PRESS;
                                return tmp;
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
}