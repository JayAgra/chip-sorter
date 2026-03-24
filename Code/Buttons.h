//
//  Buttons.h
//  SLDP
//
//  Created by Jayen Agrawal on 3/10/26.
//

#ifndef BUTTONS_H
#define BUTTONS_H

#include <avr/pgmspace.h>

static const uint16_t LONG_PRESS = 750;

namespace StackLabs {
    namespace Buttons {
        struct Press {
            int button;
            bool held;
        };
        
        /**
         * @brief   Takes ownership of the Multiplexer and returns the next
         *          button press (> 20ms). Blocks any use of Stepper MicroStep.
         * 
         * @return  Returns a Press structure, containing the button pressed
         *          (int button) and if the button was held (bool held).
        */
        Press getPress();
    }
}

#endif