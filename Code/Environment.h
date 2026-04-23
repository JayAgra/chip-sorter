//
//  Environment.h
//  SLDP
//
//  Created by Jayen Agrawal on 3/24/26.
//

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#define HARDWARE (const char *)"1.0.0 4/13"
#define FIRMWARE (const char *)"1.0.0 4/13"
#define SOFTWARE (const char *)"1.0.0 4/13"
#define PLATFORM (const char *)"AVR"

#define DEBUG_FIRMWARE false

#if DEBUG_FIRMWARE

#include <Arduino.h>

Print& operator<<(Print& p, const char *value);
Print& operator<<(Print& p, int value);

#endif

#endif