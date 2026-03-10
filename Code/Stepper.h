//
//  Stepper.h
//  SLDP
//
//  Created by Jayen Agrawal on 3/10/26.
//

#ifndef STEPPER_H
#define STEPPER_H

#include "StackLabs.h"

const uint8_t TOTAL_FULL_STEPS = 200;

namespace StackLabs {
    namespace Stepper {
        class Motor {
        private:
            int step, dir, sleep, reset;
            int ms1, ms2;
            uint8_t muxRequester;
            bool clockwise;
            uint8_t speed, mode;
            uint16_t position;

            /**
             * @brief   Normalizes step position rotation to be an offset from
             *          zero that does not exceed the step count.
             * 
             * @param   add     Optional number of steps to add at this stage.
             * 
             * @return  Current set, normalized.
            */
            uint16_t normalizeRotation(uint16_t add = 0);

            /**
             * @brief   Normalizes step position such that the current position
             *          can be converted to a whole position without remainder.
             * 
             * @return  Current step, normalized to an offset from zero.
            */
            uint16_t normalizeStep();
        public:
            Motor(int step, int dir, int sleep, int reset, int ms1, int ms2,
                uint8_t muxRequester): step(step), dir(dir), sleep(sleep),
                reset(reset), ms1(ms1), ms2(ms2), clockwise(true),
                muxRequester(muxRequester), speed(4999), position(0), mode(1)
                {}
            
            /**
             * @brief   Initializes the motor.
            */
            void setup();

            /**
             * @brief   Sets the motor speed.
             * 
             * @param   speed   Motor rotation speed. Value [0-5000).
            */
            uint16_t setSpeed(uint16_t speed);

            /**
             * @brief   Moves a specific number of steps. 200 steps when in
             *          full step mode, 400 in 1/2 step, and 800 in 1/4 step.
             * 
             * @param   steps       Number of steps to move.
             * @param   accelLimit  Limit of motor acceleration.
             * 
             * @return  Current step, normalized to an offset from zero.
            */
            uint16_t move(uint16_t steps, uint16_t accelLimit = 20);

            /**
             * @brief   Set the step mode to full, 1/2, or 1/4. Other step
             *          modes are not supported as a multiplexer is used to
             *          control MS, so only one pin can be set at a time.
             *          Full = 200 steps around, 1/2 = 400, 1/4 = 800. Calling
             *          releaseStepMode after is REQUIRED to release the
             *          multiplexer to use by other requesters. Failing to do
             *          so will soft brick the system. Setting to full is not
             *          needed, as it is the default.
             * 
             * @param   mode    The denominator for the mode. Full = 1,
             *                  1/2 = 2, and 1/4 = 4. Other values invalid.
             * 
             * @return  Current step, normalized to an offset from zero.
            */
            uint16_t setStepMode(uint8_t mode);

            /**
             * @brief   Releases the step mode so the multiplexer may be used.
             *          The step mode becomes full after this is called.
             * 
             * @return  Current step, normalized to an offset from zero.
            */
            uint16_t releaseStepMode();

            /**
             * @brief   Set the direction for the stepper motor to turn in.
             * 
             * @param   clockwise   true = clockwise, false = counterclockwise.
             * 
             * @return  true = set clockwise, false = set counterclockwise.
            */
            bool setDirection(bool clockwise);

            /**
             * @brief   Stops the motor where it is. Not ideal; try something
             *          else if possible.
            */
            void stop();

            /**
             * @brief   Resets the motor position to "zero".
            */
            void resetPosition();

            /**
             * @brief   Sets the sleep mode on the controller.
             * 
             * @param   sleep   true to sleep, false to wake.
            */
           void setSleep(bool sleep);
        };

        extern Motor Stepper1;
        extern Motor Stepper2;

        /**
         * @brief   Quick way to call setup on both steppers.
        */
        void setupBothSteppers();
    }
}

#endif