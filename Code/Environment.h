//
//  Environment.h
//  SLDP
//
//  Created by Jayen Agrawal on 3/24/26.
//

#include "StackLabs.h"

#include <Arduino.h>

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#define HARDWARE 100
#define FIRMWARE 100
#define SOFTWARE 100

#define DEBUG_FIRMWARE true

#if DEBUG_FIRMWARE

template<typename T>
Print& operator<<(Print& p, T value);

#endif

#endif