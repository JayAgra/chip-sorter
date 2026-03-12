//
//  Stepper.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/10/26.
//

#include "Stepper.h"
#include "Multiplexer.h"

#include <Arduino.h>

StackLabs::Stepper::Motor StackLabs::Stepper::Stepper1(2, 3, 7, 6, 0, 1, 2);
StackLabs::Stepper::Motor StackLabs::Stepper::Stepper2(4, 5, 9, 8, 3, 4, 3);

void StackLabs::Stepper::Motor::setup() {
    pinMode(step, OUTPUT);
    pinMode(dir, OUTPUT);
    pinMode(sleep, OUTPUT);
    pinMode(reset, OUTPUT);

    digitalWrite(sleep, HIGH);
    digitalWrite(reset, HIGH);
}

uint16_t StackLabs::Stepper::Motor::setSpeed(uint16_t speed) {
    if (speed < 5000) {
        this->speed = speed;
    }
    return this->speed;
}

uint16_t StackLabs::Stepper::Motor::normalizeRotation(uint16_t add = 0) {
    this->position = (this->position + add) % (TOTAL_FULL_STEPS * this->mode);
    return this->position;
}

uint16_t StackLabs::Stepper::Motor::move(uint16_t steps, uint16_t accelLimit = 20) {
    if (steps == 0) {
        return this->position;
    }

    digitalWrite(dir, this->clockwise);

    // assume we're stopped, set lowest supported speed
    uint16_t currentSpeed = 4999;
    uint16_t targetSpeed = 5000 - this->speed;

    // acceleration phase
    for (uint16_t i = 0; i < steps / 2; ++i) {
        digitalWrite(step, HIGH);
        delayMicroseconds(currentSpeed);
        digitalWrite(step, LOW);
        delayMicroseconds(currentSpeed);
        if (currentSpeed > (5000 - this->speed * 2)) {
            currentSpeed -= accelLimit;
        }
    }

    // constant phase
    for (uint16_t i = steps / 2; i < (steps - steps / 2); ++i) {
        digitalWrite(step, HIGH);
        delayMicroseconds(currentSpeed);
        digitalWrite(step, LOW);
        delayMicroseconds(currentSpeed);
    }

    // deceleration phase
    for (uint16_t i = (steps - steps / 2); i < steps; ++i) {
        digitalWrite(step, HIGH);
        delayMicroseconds(currentSpeed);
        digitalWrite(step, LOW);
        delayMicroseconds(currentSpeed);
        if (currentSpeed < (5000 - this->speed)) {
            currentSpeed += accelLimit;
        }
    }

    this->normalizeRotation(steps);

    return this->position;
}

uint16_t StackLabs::Stepper::Motor::normalizeStep() {
    if (this->mode == 1) {
        return this->position;
    }

    uint16_t newPosition;

    // normalize position of all non-full step settings to be even
    if (this->position % 2 == 1) {
        newPosition = this->move(1);
    }

    // normalize position of quarter-step setting to be full
    if (this->mode == 4 && this->position % 4 == 2) {
        newPosition = this->move(2);
    }

    this->position = newPosition;
    this->normalizeRotation();

    return this->position;
}

uint16_t StackLabs::Stepper::Motor::setStepMode(uint8_t mode) {
    if (mode == 1) {
        return this->releaseStepMode();
    } else if (mode != 2 && mode != 4) {
        return this->position;
    }

    uint16_t newStep = this->normalizeStep();

    Multiplexer::takeOwnership(this->muxRequester, OUTPUT);
    if (mode == 2) {
        Multiplexer::selectChannel(this->muxRequester, this->ms2);
        Multiplexer::write(this->muxRequester, LOW);
        Multiplexer::selectChannel(this->muxRequester, this->ms1);
    } else {
        Multiplexer::selectChannel(this->muxRequester, this->ms1);
        Multiplexer::write(this->muxRequester, LOW);
        Multiplexer::selectChannel(this->muxRequester, this->ms2);
    }
    Multiplexer::write(this->muxRequester, HIGH);

    this->position = (newStep / this->mode);
    this->position *= mode;
    this->mode = mode;
    this->normalizeRotation();
    return this->position;
}

uint16_t StackLabs::Stepper::Motor::releaseStepMode() {
    if (this->mode == 1) {
        return this->position;
    }

    uint16_t newStep = this->normalizeStep();

    Multiplexer::selectChannel(this->muxRequester, this->ms1);
    Multiplexer::write(this->muxRequester, LOW);
    Multiplexer::selectChannel(this->muxRequester, this->ms2);
    Multiplexer::write(this->muxRequester, LOW);
    Multiplexer::releaseOwnership(this->muxRequester);

    this->position = (newStep / this->mode);
    this->normalizeRotation();

    return this->position;
}

bool StackLabs::Stepper::Motor::setDirection(bool clockwise) {
    this->clockwise = clockwise;
    digitalWrite(dir, this->clockwise);
}

void StackLabs::Stepper::Motor::stop() {
    digitalWrite(step, LOW);
}

void StackLabs::Stepper::Motor::resetPosition() {
    digitalWrite(reset, LOW);
    delayMicroseconds(10);
    digitalWrite(reset, HIGH);
}

void StackLabs::Stepper::Motor::setSleep(bool sleep) {
    digitalWrite(sleep, ~sleep);
}

namespace StackLabs {
    namespace Stepper {
        void setupBothSteppers() {
            StackLabs::Stepper::Stepper1.setup();
            StackLabs::Stepper::Stepper2.setup();
        }
    }
}