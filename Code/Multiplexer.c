//
//  Multiplexer.c
//  SLDP
//
//  Created by Jayen Agrawal on 3/10/26.
//

#include "Multiplexer.h"

static const int MUX_SIGNAL = 14u;
static const int MUX_CONTROL[] = {10, 11, 12, 13};

// 0 = free, 1 = buttons, 2 = motor 1, 3 = motor 2
static uint8_t owner = 0;
static uint8_t mode = 0x1;

static bool run_auth_check(uint8_t requester) {
    if (requester != owner) {
        #if DEBUG_FIRMWARE
        Serial << "Requester unauthorized\n";
        #endif
        return false;
    }

    return true;
}

static bool run_operation_check(bool write) {
    if ((mode != 0x1) == write) {
        #if DEBUG_FIRMWARE
        Serial << "Mode incompatible with operation\n";
        #endif
        return false;
    }
    
    return true;
}

void setup_mux() {
    for (size_t i = 0; i < 4; ++i) {
        pinMode(MUX_CONTROL[i], 0x1);
    }
    pinMode(MUX_SIGNAL, mode);
}

bool take_mux_ownership(uint8_t requester, uint8_t setMode) {
    if (owner == 0 || owner == requester) {
        owner = requester;
        mode = setMode;
        pinMode(MUX_SIGNAL, mode);
        return true;
    }
    
    #if DEBUG_FIRMWARE
    Serial << "Ownership request from " << requester << " rejected; "
            << owner << " is the owner\n";
    #endif

    return false;
}

bool release_mux_ownership(uint8_t requester) {
    if (!run_auth_check(requester)) return false;
    
    owner = 0;
    return true;
}

bool select_mux_channel(uint8_t requester, uint8_t channel) {
    if (!run_auth_check(requester)) return false;
    if (channel > 15) {
        #if DEBUG_FIRMWARE
        Serial << "Invalid channel requested [0, 15]\n";
        #endif
        return false;
    }

    for (int i = 0; i < 4; ++i) {
        uint8_t value = (channel >> i) & 1;
        digitalWrite(MUX_CONTROL[(size_t) i], value);
    }

    delay(1);

    return true;
}

int8_t read_mux(uint8_t requester) {
    if (!run_auth_check(requester)) return false;
    if (!run_operation_check(false)) return 0xff;

    return digitalRead(MUX_SIGNAL);
}

bool write_mux(uint8_t requester, uint8_t value) {
    if (!run_auth_check(requester)) return false;
    if (!run_operation_check(true)) return 0xff;

    digitalWrite(MUX_SIGNAL, value);

    return true;
}
