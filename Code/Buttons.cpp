//
//  Buttons.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/10/26.
//

#include "Buttons.h"

static const uint8_t MUX_ID = 1;
static unsigned long startTimes[4];

namespace StackLabs {
    namespace Buttons {
        Press getPress() {
            bool muxOwnership = false;

            // wait until we own the multiplexer
            while (!muxOwnership) {
                muxOwnership = Multiplexer::takeOwnership(MUX_ID, 0x2);
                #if DEBUG_FIRMWARE
                Serial << "Mux ownership status:  " << muxOwnership << "\n";
                #endif
                delay(100);
            }

            while (true) {
                for (int i = 0; i < 4; ++i) {
                    Multiplexer::selectChannel(MUX_ID, i + 8);
                    int8_t buttonValue = Multiplexer::read(MUX_ID);

                    #if DEBUG_FIRMWARE
                    // Serial << "Value of button " << i << ": " << buttonValue
                    //        << "\n";
                    #endif
                    
                    switch (buttonValue) {
                    case 0:
                        if (startTimes[i] == 0) {
                            startTimes[i] = millis();
                            #if DEBUG_FIRMWARE
                            Serial << "Button pressed: " << i << "\n";
                            #endif
                        }
                        break;
                    default: // 1, not pressed
                        if (startTimes[i] != 0) {
                            unsigned long pressLength =
                                millis() - startTimes[i];
                            startTimes[i] = 0;
                            // try to ignore noise
                            if (pressLength > 20) {
                                Multiplexer::releaseOwnership(MUX_ID);
                                Press tmp = Press();
                                // flip least significant digit to map pin no.
                                tmp.button = i ^ 2;
                                tmp.held = pressLength > LONG_PRESS;
                                #if DEBUG_FIRMWARE
                                Serial << "Button released: " << i << "\n";
                                #endif
                                return tmp;
                            }
                        }
                        break;
                    }
                    delay(2);
                }
            }
        }
    }
}

/*
    0 - Button "3"
    1 - Button "4"
    2 - Button "1"
    3 - Button "2"
*/