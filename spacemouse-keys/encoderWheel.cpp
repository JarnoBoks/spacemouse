/*
 * This code gives the space mouse the ability to include an encoder wheel.
 * The encoder gives positions but the space mouse reports velocities.
 * Therefore, we calculate a filtered derivative from the encoder position and
 * replace the desired velocity from the original space mouse.
 *
 * Based on the idea by JoseLuisGZA, rewritten by AndunHH
 */

#include <Arduino.h>
#include "config.h"
#if ROTARY_AXIS > 0 or ROTARY_KEYS > 0
#include "encoderWheel.h"

// Include Encoder library by Paul Stoffregen
#include <Encoder.h>

Encoder myEncoder(ENCODER_CLK, ENCODER_DT);

int32_t previousEncoderValue = 0;
int32_t newEncoderValue; // Store encoder readings
int32_t delta = 0;       // Tracks encoder increments when turned

int zoomIterator = ECHOES; // Counter for echoing the delta through a number of loops for a smoother zoom animation
float simpull;             // calculated velocity of the encoder wheel

void initEncoderWheel() {
    // Read initial value from encoder
    newEncoderValue = myEncoder.read();
}

/**
 * @brief Calculate the encoder wheel and update the result in the velocity array
 * @param velocity Array with the velocity, which gets updated at position ROTARY_AXIS-1
 * @param debug Generate a debug output if debug=9
 */
void calcEncoderWheel(Kinematics &SMKIN, int debug) {
    static int factor = 100; //
    // Read encoder and check if it changed
    newEncoderValue = myEncoder.read();
    if (newEncoderValue != previousEncoderValue) {
        // The position of the encoder changed, how much did it change?
        delta = newEncoderValue - previousEncoderValue;
        previousEncoderValue = newEncoderValue;
        zoomIterator = 0;
    }

    // Distribute encoder delta through the echoes in the loop and based on simulated axis chosen by the user
    // Faded intensity for echoing the encoder reading.
    if (zoomIterator < ECHOES) {
        factor = 100 - ((zoomIterator * 100) / ECHOES); // factor shall be percent: between 0 and 100
        simpull = (factor * SIMSTRENGTH) / 100 * delta;
        zoomIterator++; // iterate

        // Add the velocity of the encoder wheel to one of the 6 axis
        // The ROTARY_AXIS definition is one above the array definition used for the velocity array (see calibration.h)
        // Therefore ROTARY_AXIS-1 is used to change the velocity value
        SMKIN.SetVelocity((enumAxis_t)(ROTARY_AXIS - 1), SMKIN.GetVelocity((enumAxis_t)(ROTARY_AXIS - 1)) + simpull);
    } else {
        // fading has ended
        simpull = 0;
    }

    if (debug == 9) {
        // create debug output
        Serial.print("Enc Val: ");
        Serial.print(newEncoderValue);
        Serial.print(", factor: ");
        Serial.print(factor);
        Serial.print(", simpull: ");
        Serial.println(simpull);
    }
}

/// @brief Read out the encoder and treat as keystroke
/// @param keyState overwrite some keys with encoder movement
/// @param debug Generate a debug output if debug=9
void calcEncoderAsKey(SpaceKeys *SMKEYS, int debug) {
    // read encoder
    newEncoderValue = myEncoder.read();
    if (newEncoderValue != previousEncoderValue) {
        // If the position changed, add this to delta. As long as delta != 0, report the key as pressed
        delta = (newEncoderValue - previousEncoderValue) * ROTARY_KEY_STRENGTH + delta;
        previousEncoderValue = newEncoderValue;

        if (debug == 9) {
            // create debug output
            Serial.print("Enc Val: ");
            Serial.println(newEncoderValue);
        }
    }

    if (delta > 0) {
        // press the button for some small time
        SMKEYS->SetKeyState(ROTARY_KEY_IDX_A, 1);
        delta--;
    } else if (delta < 0) {
        // press the button for some small time
        SMKEYS->SetKeyState(ROTARY_KEY_IDX_B, 1);
        delta++;
    } else {
        // no movement, reset the key state
        SMKEYS->SetKeyState(ROTARY_KEY_IDX_A, 0);
        SMKEYS->SetKeyState(ROTARY_KEY_IDX_B, 0);
    }
}
#endif // whole file is only implemented #if ROTARY_AXIS > 0 or ROTARY_KEYS > 0