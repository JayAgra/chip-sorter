//
//  Multiplexer.h
//  SLDP
//
//  Created by Jayen Agrawal on 3/10/26.
//

#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#include "Environment.h"

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif
    /**
     * @brief   Initializes the multiplexer.
    */
    void setup_mux();

    /**
     * @brief   Grants ownership of the multiplexer, if available.
     * 
     * @param   requester   Requester ID. 1 for buttons, 2 for Stepper 1, 3
     *                      for Stepper 2. 0 is an invalid ID.
     * @param   setMode     Mode to set the signal pin to.
     * 
     * @return  true if ownership was granted, false if not.
    */
    bool take_mux_ownership(uint8_t requester, uint8_t setMode);

    /**
     * @brief   Releases ownership of the multiplexer, if caller has it.
     * 
     * @param   requester   Requester ID.
     * 
     * @return  true if ownership was released, false if the requester did
     *          not have it in the first place.
    */
    bool release_mux_ownership(uint8_t requester);

    /**
     * @brief   Selects a channel.
     * 
     * @param   requester   Requester ID.
     * @param   channel     Channel to set. 0-15 only.
     * 
     * @return  true if the channel was selected. false if the requester
     *          was not authorized or if the channel was out of bounds.
    */
    bool select_mux_channel(uint8_t requester, uint8_t channel);

    /**
     * @brief   Reads a data value from the current channel.
     * 
     * @param   requester   Requester ID.
     * 
     * @return  -1 if not authorized, or if the multiplexer is in output
     *          mode (change mode using takeOwnership again). 0 or 1 based
     *          on the value read (0 = LOW, 1 = HIGH).
    */
    int8_t read_mux(uint8_t requester);

    /**
     * @brief   Writes a data value to the current channel.
     * 
     * @param   requester   Requester ID.
     * @param   value       Value to write, must be HIGH or LOW.
     * 
     * @return  false if requester was not authorized or if the multiplexer
     *          is in input mode (change mode using takeOwnership again).
    */
    bool write_mux(uint8_t requester, uint8_t value);
#ifdef __cplusplus
}
#endif
#endif