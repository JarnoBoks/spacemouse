// This file contains all functions to calculate the kinematics

// The user specific settings, like pin mappings or special configuration variables and sensitivities are stored in config.h.
// Please open config_sample.h, adjust your settings and save it as config.h
#include "config.h"
#include <Arduino.h>

// Include math operators for doing better calculation algorithms. Arduino math is a standard library already included.
#include <math.h>
#define sign(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0)) // Define Signum Function

#include "calibration.h"
#include "kinematics.h"
#ifdef EEPROM_CALIBRATION
#include "sensitivity.h"
#endif

#ifndef EEPROM_CALIBRATION
/// @brief Function to modify the input value according to different mathematic modes. Choose the mathematical function in config.h as modFunc
/// @param x input between -350 and +350
/// @return output between -350 and +350
int modifierFunction(int x) {
    // making sure function input never exedes range of -350 to 350
    x = constrain(x, -350, 350);
    double result;
#if (MODFUNC == 1)
    // using squared function y = x^2*sign(x)
    result = 350 * pow(x / 350.0, 2) * sign(x); // sign putting out -1 or 1 depending on sign of value. (Is needed because x^2 will always be positive)
#elif (MODFUNC == 2)
    // using tan function: tan(x)
    result = 350 * tan(x / 350.0);
#elif (MODFUNC == 3)
    // using squared tan function: tan(x^2*sign(x))
    result = 350 * tan(pow(x / 350.0, 2) * sign(x)); // sign putting out -1 or 1 depending on sign of value. (Is needed because x^2 will always be positive)
#elif (MODFUNC == 4)
    // using cubed tan function: tan(x^3)
    result = 350 * tan(pow(x / 350.0, 3));
#else
    // MODFUNC == 0 or others...
    // no modification
    result = x;
#endif

    // make sure values between-350 and 350 are allowed
    result = constrain(result, -350, 350);

    // converting doubles to int again
    return (int)round(result);
}
#else
/**
 * @brief Function to modify the input value according to different mathematic modes. Choose the mathematical function in config.h as modFunc
 * @param x input between -350 and +350
 * @param modFunc The modifier function to use
 * @return output between -350 and +350
 */
int _modifierFunction(int x, uint8_t modFunc) {
    // making sure function input never exceeds range of -350 to 350
    x = constrain(x, -350, 350);
    double result;

    switch (modFunc) {
    case 1:
        // using squared function y = x^2*sign(x)
        result = 350 * pow(x / 350.0, 2) * sign(x); // sign putting out -1 or 1 depending on sign of value. (Is needed because x^2 will always be positive)
        break;
    case 2:
        // using tan function: tan(x)
        result = 350 * tan(x / 350.0);
        break;
    case 3:
        // using squared tan function: tan(x^2*sign(x))
        result = 350 * tan(pow(x / 350.0, 2) * sign(x)); // sign putting out -1 or 1 depending on sign of value. (Is needed because x^2 will always be positive)
        break;
    case 4:
        // using cubed tan function: tan(x^3)
        result = 350 * tan(pow(x / 350.0, 3));
        break;
    default:
        result = x;
    }

    // make sure values between-350 and 350 are allowed
    result = constrain(result, -350, 350);

    // converting doubles to int again
    return (int)round(result);
}
#endif

/**
 *  @brief Calculate the kinematic of the three axis from the eight sensors
 *  @param centered pointer to the array containing the eight centered values from the axis of the 4 joysticks or the 8 Hall Effect sensors
 *  @param velocity pointer to the result array that will contain the translational and rotational motions
 */
void calculateKinematic(SpaceMouseHW_ &SMHW, int16_t *velocity) {
    // Retrieve raw kinematics from sensors (joystick or hall effect)
    SMHW.CalculateKinematicSensors(velocity);

    // transX - Apply sensitivity & recalculate with modifier function.
    velocity[TRANSX] = velocity[TRANSX] / ((float)TRANSX_SENSITIVITY);
    velocity[TRANSX] = modifierFunction(velocity[TRANSX]); // recalculate with modifier function

    // transY - Apply sensitivity & recalculate with modifier function.
    velocity[TRANSY] = velocity[TRANSY] / ((float)TRANSY_SENSITIVITY);
    velocity[TRANSY] = modifierFunction(velocity[TRANSY]); // recalculate with modifier function

    // transZ
    // REVIEW - Check if this will be OK for the HallE and Joystick version, while the negative & positive moment is inverted in the RAW ADC values!!
    if (velocity[TRANSZ] < 0) {
        velocity[TRANSZ] = modifierFunction(velocity[TRANSZ] / ((float)NEG_TRANSZ_SENSITIVITY)); // recalculate with modifier function
        if (abs(velocity[TRANSZ]) < GATE_NEG_TRANSZ) {
            velocity[TRANSZ] = 0;
        }
    } else {                                                                                         // pulling the knob upwards is much heavier... smaller factor
        velocity[TRANSZ] = constrain(velocity[TRANSZ] / ((float)POS_TRANSZ_SENSITIVITY), -350, 350); // no modifier function, just constrain linear!
    }

    // rotX - Apply sensitivity, recalculate with modifier function and apply gate
    velocity[ROTX] = velocity[ROTX] / ((float)ROTX_SENSITIVITY);
    velocity[ROTX] = modifierFunction(velocity[ROTX]); // recalculate with modifier function
    if (abs(velocity[ROTX]) < GATE_ROTX) {
        velocity[ROTX] = 0;
    }

    // rotY - Apply sensitivity, recalculate with modifier function and apply gate
    velocity[ROTY] = velocity[ROTY] / ((float)ROTY_SENSITIVITY);
    velocity[ROTY] = modifierFunction(velocity[ROTY]); // recalculate with modifier function
    if (abs(velocity[ROTY]) < GATE_ROTY) {
        velocity[ROTY] = 0;
    }

    // rotY - Apply sensitivity, recalculate with modifier function and apply gate
    velocity[ROTZ] = velocity[ROTZ] / ((float)ROTZ_SENSITIVITY);
    velocity[ROTZ] = modifierFunction(velocity[ROTZ]); // recalculate with modifier function
    if (abs(velocity[ROTZ]) < GATE_ROTZ) {
        velocity[ROTZ] = 0;
    }

#ifndef EEPROM_CALIBRATION
// Invert directions if needed
#if INVX > 0
    velocity[TRANSX] = velocity[TRANSX] * -1;
#endif
#if INVY > 0
    velocity[TRANSY] = velocity[TRANSY] * -1;
#endif
#if INVZ > 0
    velocity[TRANSZ] = velocity[TRANSZ] * -1;
#endif
#if INVRX > 0
    velocity[ROTX] = velocity[ROTX] * -1;
#endif
#if INVRY > 0
    velocity[ROTY] = velocity[ROTY] * -1;
#endif
#if INVRZ > 0
    velocity[ROTZ] = velocity[ROTZ] * -1;
#endif
#else
    if (GET_INVERSION(inversions, AX_INVX))
        velocity[TRANSX] *= -1;
    if (GET_INVERSION(inversions, AX_INVY))
        velocity[TRANSY] *= -1;
    if (GET_INVERSION(inversions, AX_INVZ))
        velocity[TRANSZ] *= -1;
    if (GET_INVERSION(inversions, AX_INVRX))
        velocity[ROTX] *= -1;
    if (GET_INVERSION(inversions, AX_INVRY))
        velocity[ROTY] *= -1;
    if (GET_INVERSION(inversions, AX_INVRZ))
        velocity[ROTZ] *= -1;
#endif
} // end calculateKinematic

/**
 *  @brief Switch position of X and Y values
 *
 *  @param velocity pointer to velocity array
 */
void switchXY(int16_t *velocity) {
    int16_t tmp = 0;
    tmp = velocity[TRANSX];
    velocity[TRANSX] = velocity[TRANSY];
    velocity[TRANSY] = tmp;

    tmp = velocity[ROTX];
    velocity[ROTX] = velocity[ROTY];
    velocity[ROTY] = tmp;
}

/**
 *  @brief Switch position of Y and Z values
 *
 *  @param velocity pointer to velocity array
 */
void switchYZ(int16_t *velocity) {
    int16_t tmp = 0;
    tmp = velocity[TRANSY];
    velocity[TRANSY] = velocity[TRANSZ];
    velocity[TRANSZ] = tmp;

    tmp = velocity[ROTY];
    velocity[ROTY] = velocity[ROTZ];
    velocity[ROTZ] = tmp;
}

/**
 *  @brief  Check if translation or rotation is dominant and set the other values to zero to allow exclusively rotation or translation
 *          to avoid issues with classics joysticks.
 *
 *  @param velocity pointer to velocity array
 */
void exclusiveMode(int16_t *velocity) {
    uint16_t totalRot = abs(velocity[ROTX]) + abs(velocity[ROTY]) + abs(velocity[ROTZ]);
    uint16_t totalTrans = abs(velocity[TRANSX]) + abs(velocity[TRANSY]) + abs(velocity[TRANSZ]);

    if (totalRot > totalTrans) {
        velocity[TRANSX] = 0;
        velocity[TRANSY] = 0;
        velocity[TRANSZ] = 0;
    } else {
        velocity[ROTX] = 0;
        velocity[ROTY] = 0;
        velocity[ROTZ] = 0;
    }
}
