//
//  Buttons.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/10/26.
//

#include "Buttons.h"

#define MAX_OWNERSHIP_WAIT 120 // 1 minute
#define MAX_USER_WAIT 12000 // 2 minutes no complete interaction

static const uint8_t MUX_REQUESTER_ID = 1;
static unsigned long startTimes[4];

namespace StackLabs {
    namespace Buttons {
        Press getPress() {
            bool muxOwnership = false;
            int ownershipWait = 0, userWait = 0;
            
            // try to get it
            muxOwnership =
                Multiplexer::takeOwnership(MUX_REQUESTER_ID, INPUT_PULLUP);

            // wait until we own the multiplexer
            while (!muxOwnership && ownershipWait < MAX_OWNERSHIP_WAIT) {
                muxOwnership =
                    Multiplexer::takeOwnership(MUX_REQUESTER_ID, INPUT_PULLUP);
                ++ownershipWait;
                delay(500);
            }

            while (userWait < MAX_USER_WAIT) {
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
                        } else {
                            ++userWait;
                        }
                        break;
                    default:
                        ++userWait;
                        break;
                    }
                }
                delay(10);
            }
        }
    }
}