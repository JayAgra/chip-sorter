//
//  Stepper.cpp
//  SLDP
//
//  Created by Jayen Agrawal on 3/10/26.
//

#include "Stepper.h"

using namespace StackLabs;

Stepper::Motor Stepper::Stepper1(2, 3, 8, 6, 0, 1, 2, 0);
Stepper::Motor Stepper::Stepper2(4, 5, 9, 7, 3, 4, 3, 1);

void Stepper::Motor::setup() {
    pinMode(step, OUTPUT);
    pinMode(dir, OUTPUT);
    pinMode(sleep, OUTPUT);
    pinMode(reset, OUTPUT);

    digitalWrite(sleep, HIGH);
    digitalWrite(reset, HIGH);
    this->setDirection(MotorDirection::CLOCKWISE);

    this->releaseStepMode();
    this->setSpeed(0x444u);
    this->stop();
}

uint16_t Stepper::Motor::setSpeed(uint16_t speed) {
    // im just guessing on speed will test in a bit
    if (speed <= STEPPER_MAX_DELAY && speed >= STEPPER_MIN_DELAY) {
        this->speed = speed;
    }

    return this->speed;
}

uint16_t Stepper::Motor::normalizeRotation(uint16_t add = 0) {
    if (this->direction == MotorDirection::CLOCKWISE) {
        this->position = (this->position + add) % (TOTAL_FULL_STEPS * this->mode);
    } else {
        this->position = (this->position - add) % (TOTAL_FULL_STEPS * this->mode);
    }

    if (this->id == 0) Data::write(EEPROM_TOP_POS, this->position, true);
    if (this->id == 1) Data::write(EEPROM_BOTTOM_POS, this->position, true);

    return this->position;
}

uint16_t Stepper::Motor::move(uint16_t steps, uint8_t accelLimit = 40) {
    if (steps < 2) return this->position;

    #if DEBUG_FIRMWARE
    Serial << "Moving stepper " << steps << " steps.\n";
    #endif

    digitalWrite(dir, this->direction);

    uint16_t accelSteps = max(1, steps / 3);
    uint16_t decelSteps = accelSteps;
    uint16_t constSteps = steps - (accelSteps * 2);

    uint16_t currentDelay = STEPPER_MAX_DELAY;

    auto pulse = [&]() {
        digitalWrite(step, HIGH);
        delayMicroseconds(currentDelay);
        digitalWrite(step, LOW);
        delayMicroseconds(currentDelay);
    };

    for (uint16_t i = 0; i < accelSteps; ++i) {
        pulse();
        if (currentDelay > this->speed + accelLimit) {
            currentDelay -= accelLimit;
        } else {
            currentDelay = this->speed;
        }
    }

    for (uint16_t i = 0; i < constSteps; ++i) {
        pulse();
    }

    for (uint16_t i = 0; i < decelSteps; ++i) {
        pulse();
        if (currentDelay < STEPPER_MAX_DELAY - accelLimit) {
            currentDelay += accelLimit;
        } else {
            currentDelay = STEPPER_MAX_DELAY;
        }
    }

    if (this->mode == 1) this->normalizeRotation(steps);

    this->stop();

    return this->position;
}

uint16_t Stepper::Motor::vectorMove(int16_t steps, uint8_t accelLimit = 40) {
    if (abs(steps) < 2) return this->position;
    this->setDirection((steps > 0) ? MotorDirection::CLOCKWISE :
        MotorDirection::COUNTERCLOCKWISE);
    this->move(abs(steps % 200), accelLimit);
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

    take_mux_ownership(this->muxRequester, OUTPUT);
    if (mode == 2) {
        select_mux_channel(this->muxRequester, this->ms2);
        write_mux(this->muxRequester, LOW);
        select_mux_channel(this->muxRequester, this->ms1);
    } else {
        select_mux_channel(this->muxRequester, this->ms1);
        write_mux(this->muxRequester, LOW);
        select_mux_channel(this->muxRequester, this->ms2);
    }
    write_mux(this->muxRequester, HIGH);

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

    take_mux_ownership(this->muxRequester, OUTPUT);
    select_mux_channel(this->muxRequester, this->ms1);
    write_mux(this->muxRequester, LOW);
    select_mux_channel(this->muxRequester, this->ms2);
    write_mux(this->muxRequester, LOW);
    release_mux_ownership(this->muxRequester);

    this->position = (newStep / this->mode);
    this->normalizeRotation();

    #if DEBUG_FIRMWARE
    Serial << "Released step mode & mux ownership\n";
    #endif

    return this->position;
}

bool Stepper::Motor::setDirection(MotorDirection direction) {
    this->direction = direction;

    digitalWrite(dir, direction);

    delay(1);

    return direction;
}

void Stepper::Motor::stop() {
    digitalWrite(step, LOW);
    #if DEBUG_FIRMWARE
    Serial << "Forced stopped motor, pos data may be inaccurate\n";
    #endif
}

void Stepper::Motor::resetController() {
    digitalWrite(reset, LOW);
    delayMicroseconds(10u);
    digitalWrite(reset, HIGH);
}

void Stepper::Motor::home() {
    this->setDirection(MotorDirection::COUNTERCLOCKWISE);
    this->move(this->position % 200u, 10);
}

void Stepper::Motor::setSleep(bool sleep) {
    digitalWrite(sleep, ~sleep);
}

void Stepper::Motor::setZero() {
    this->position = 0;
    this->normalizeStep();
}

namespace StackLabs {
    namespace Stepper {
        void setupBothSteppers() {
            Stepper::Stepper1.setup();
            Stepper::Stepper2.setup();
        }
    }
}