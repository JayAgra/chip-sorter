//
//  Environment.h
//  SLDP
//
//  Created by Jayen Agrawal on 3/24/26.
//

#include "Environment.h"

#if DEBUG_FIRMWARE

Print& operator<<(Print& p, const char *value) {
    p.print(value);
    return p;
}

Print& operator<<(Print& p, int value) {
    p.print(value);
    return p;
}

#endif