//
//  Environment.h
//  SLDP
//
//  Created by Jayen Agrawal on 3/24/26.
//

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#define HARDWARE 100
#define FIRMWARE 100
#define SOFTWARE 100

#define DEBUG_FIRMWARE false

#if DEBUG_FIRMWARE

#include <Arduino.h>

Print& operator<<(Print& p, const char *value);
Print& operator<<(Print& p, int value);

#endif

#endif