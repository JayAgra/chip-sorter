//
//  Environment.h
//  SLDP
//
//  Created by Jayen Agrawal on 3/24/26.
//

#include "Environment.h"

#include <Arduino.h>

#if DEBUG_FIRMWARE

template<typename T>
Print& operator<<(Print& p, T value) {
    p.print(value);
    return p;
}

#endif