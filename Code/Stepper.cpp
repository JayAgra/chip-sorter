//
//  Stepper.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/10/26.
//

#include "Stepper.h"

using namespace StackLabs;

Stepper::Motor Stepper::Stepper1(2, 3, 7, 6, 0, 1, 2);
Stepper::Motor Stepper::Stepper2(4, 5, 9, 8, 3, 4, 3);

void Stepper::Motor::setup() {
    pinMode(step, OUTPUT);
    pinMode(dir, OUTPUT);
    pinMode(sleep, OUTPUT);
    pinMode(reset, OUTPUT);

    digitalWrite(sleep, HIGH);
    digitalWrite(reset, HIGH);

    this->releaseStepMode();
    this->resetPosition();
}

uint16_t Stepper::Motor::setSpeed(uint16_t speed) {
    // im just guessing on speed will test in a bit
    if (speed <= 4000) {
        this->speed = speed;
    }

    return this->speed;
}

uint16_t Stepper::Motor::normalizeRotation(uint16_t add = 0) {
    this->position = (this->position + add) % (TOTAL_FULL_STEPS * this->mode);

    return this->position;
}

uint16_t Stepper::Motor::move(uint16_t steps, uint16_t accelLimit = 20) {
    if (steps == 0) return this->position;

    #if DEBUG_FIRMWARE
    Serial << "Moving stepper " << steps << " steps.\n";
    #endif

    digitalWrite(dir, this->clockwise);

    const uint16_t minDelay = 1000;
    const uint16_t maxDelay = 4000;
    uint16_t targetDelay = maxDelay - this->speed;
    if (targetDelay < minDelay) targetDelay = minDelay;

    uint16_t accelSteps = steps / 3;
    uint16_t decelSteps = steps / 3;
    uint16_t constSteps = steps - accelSteps - decelSteps;

    uint16_t currentDelay = maxDelay;

    auto pulse = [&]() {
        digitalWrite(step, HIGH);
        delayMicroseconds(currentDelay);
        digitalWrite(step, LOW);
        delayMicroseconds(currentDelay);
    };

    for (uint16_t i = 0; i < accelSteps; ++i) {
        pulse();
        if (currentDelay > targetDelay + accelLimit) {
            currentDelay -= accelLimit;
        } else {
            currentDelay = targetDelay;
        }
    }

    currentDelay = targetDelay;
    for (uint16_t i = 0; i < constSteps; ++i) {
        pulse();
    }

    // Deceleration phase: ramp delay back up toward maxDelay
    for (uint16_t i = 0; i < decelSteps; ++i) {
        pulse();
        if (currentDelay < maxDelay - accelLimit) {
            currentDelay += accelLimit;
        } else {
            currentDelay = maxDelay;
        }
    }

    this->normalizeRotation(steps);

    return this->position;
}

uint16_t Stepper::Motor::normalizeStep() {
    if (this->mode == 1) return this->position;

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

    #if DEBUG_FIRMWARE
    Serial << "Moved stepper to normalize\n";
    #endif

    return this->position;
}

uint16_t Stepper::Motor::setStepMode(uint8_t mode) {
    if (mode == 1) return this->releaseStepMode();
    if (mode != 2 && mode != 4) return this->position;

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

    #if DEBUG_FIRMWARE
    Serial << "Set step mode, has mux ownership\n";
    #endif

    return this->position;
}

uint16_t Stepper::Motor::releaseStepMode() {
    if (this->mode == 1) return this->position;

    uint16_t newStep = this->normalizeStep();

    Multiplexer::takeOwnership(this->muxRequester, OUTPUT);
    Multiplexer::selectChannel(this->muxRequester, this->ms1);
    Multiplexer::write(this->muxRequester, LOW);
    Multiplexer::selectChannel(this->muxRequester, this->ms2);
    Multiplexer::write(this->muxRequester, LOW);
    Multiplexer::releaseOwnership(this->muxRequester);

    this->position = (newStep / this->mode);
    this->normalizeRotation();

    #if DEBUG_FIRMWARE
    Serial << "Released step mode & mux ownership\n";
    #endif

    return this->position;
}

bool Stepper::Motor::setDirection(bool clockwise) {
    this->clockwise = clockwise;
    digitalWrite(dir, this->clockwise);

    return this->clockwise;
}

void Stepper::Motor::stop() {
    digitalWrite(step, LOW);
    #if DEBUG_FIRMWARE
    Serial << "Forced stopped motor, pos data may be inaccurate\n";
    #endif
}

void Stepper::Motor::resetPosition() {
    digitalWrite(reset, LOW);
    delayMicroseconds(10);
    digitalWrite(reset, HIGH);
}

void Stepper::Motor::setSleep(bool sleep) {
    digitalWrite(sleep, ~sleep);
}

namespace StackLabs {
    namespace Stepper {
        void setupBothSteppers() {
            Stepper::Stepper1.setup();
            Stepper::Stepper2.setup();
        }
    }
}