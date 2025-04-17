// This file contains all functions to calculate the kinematics

// The user specific settings, like pin mappings or special configuration variables and sensitivities are stored in config.h.
// Please open config_sample.h, adjust your settings and save it as config.h
#include "config.h"
#include <Arduino.h>

// Include math operators for doing better calculation algorithms. Arduino math is a standard library already included.
#include <math.h>
#define sign(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0)) // Define Signum Function

#include "kinematics.h"
#include "calibration.h"

// Include the EEPROM address map
#include "eepromStorage.h"

// Include the EEPROM standard library
#include "EEPROM.h"

#ifndef _isNaN
#define _isNaN(x) (x != x)
#endif

// Defines the default values for the sensitivities (these are used if the EEPROM is not set yet)
// The values differ between the various hardware options.
constexpr float DEF_TRANSX = 0.80;
constexpr float DEF_TRANSY = 0.99;
constexpr float DEF_POSITIVE_TRANSZ = 2.5;
constexpr float DEF_NEGATIVE_TRANSZ = 1.5;
constexpr float DEF_ROTX_SENSITIVITY = 1.20;
constexpr float DEF_ROTY_SENSITIVITY = 1.20;
constexpr float DEF_ROTZ_SENSITIVITY = 0.90;
constexpr float DEF_GATE_NEG_TRANSZ = 15.0;
constexpr float DEF_GATE_ROTX = 15.0;
constexpr float DEF_GATE_ROTY = 15.0;
constexpr float DEF_GATE_ROTZ = 15.0;

/**
 * @brief Constructor for Kinematics class
 *
 * Create the object & technically initialise members
 * Load the configuration data from the EEPROM. If EEPROM data is not set,
 * use the defaults from config.h and store those in the EEPROM.
 */
Kinematics::Kinematics() {
    // Initialize velocities
    for (uint8_t idx = transX; idx != velocityAxis_t::LENGTH; idx++) {
        _velocities[idx] = 0;
    }

    // Read modFunc from EEPROM, use the default configured value if the byte is not set.
    _modFunc = _readByte(EEPROM_ADDRESS_MODFUNC, _modFunc);

    // Read inversions from EEPROM, use the default configured value if the byte is not set.
    _inversions = _readByte(EEPROM_ADDRESS_INVERSION, _inversions);

    // Read sensitivities from EEPROM
    _loadVelocitySensitivities();
}

// TODO - Read sensitivities, inversions from the EEPROM
Kinematics::~Kinematics() {};

/**
 *  @brief Calculate the kinematic of the three axis from the eight sensors
 *  @param centered pointer to the array containing the eight centered values from the axis of the 4 joysticks or the 8 Hall Effect sensors
 *  @param velocity pointer to the result array that will contain the translational and rotational motions
 */
void Kinematics::CalculcateKinematic(SpaceMouseHW_ &Mouse_Hardware) {

    // Get raw kinematics from sensors
    Mouse_Hardware.CalculateKinematicSensors(_velocities);

    // Loop over the velocity axis, apply sensitivity & modifier function, and invert the velocities if necessary.
    for (uint8_t axs = 0; axs != velocityAxis_t::LENGTH; axs++) {
        float sens = 1.0; // TODO - use pointer
        int8_t gate = 0;
        int8_t mfunc = -1;

        if (_velocities[axs] < 0) {
            sens = _sensitivities[axs].neg_sensitivity;
            gate = _sensitivities[axs].neg_gate;
            mfunc = _sensitivities[axs].neg_modfunc;
        } else {
            sens = _sensitivities[axs].pos_sensitivity;
            gate = _sensitivities[axs].pos_gate;
            mfunc = _sensitivities[axs].pos_modfunc;
        }

        // apply the sensitivity for this axis & direction
        _velocities[axs] /= sens;

        // Apply the modifier function for this axis, override the default one if necessary
        _velocities[axs] = _modifierFunction(_velocities[axs], mfunc);

        // Apply any gate for this axis.
        if (abs(_velocities[axs]) < gate) {
            _velocities[axs] = 0;
        }

        // Invert the motion if necessary
        if (GET_INVERSION(_inversions, axs)) {
            _velocities[axs] *= -1;
        }
    }
}

void Kinematics::PrintVelocities() {
    char debugOutputBuffer[20];

    for (int i = 0; i < velocityAxis_t::LENGTH; i++) {
        sprintf(debugOutputBuffer, "%2.2s: %4d ", _velocityNames[i], _velocities[i]);
        Serial.print(debugOutputBuffer);
    }
}

int16_t Kinematics::GetVelocity(velocityAxis_t velocity) {
    return _velocities[velocity];
}

/**
 * @brief Function to modify the input value according to different mathematic modes. Choose the mathematical function in config.h as modFunc
 * @param x input between -350 and +350
 * @return output between -350 and +350
 */

int Kinematics::_modifierFunction(int x, int8_t modfunc_override) {
    // making sure function input never exceeds range of -350 to 350
    x = constrain(x, -350, 350);
    double result;

    // if modfunc_override is set, use that value instead of the stored one
    int8_t modfunc_usage = (modfunc_override == -1) ? _modFunc : modfunc_override;

    switch (modfunc_usage) {
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

/**
 *  @brief Switch position of X and Y values
 *
 *  @param velocity pointer to velocity array
 */
void Kinematics::SwitchXY() {
    int16_t tmp = 0;
    tmp = _velocities[transX];
    _velocities[transX] = _velocities[transY];
    _velocities[transY] = tmp;

    tmp = _velocities[rotX];
    _velocities[rotX] = _velocities[rotY];
    _velocities[rotY] = tmp;
}

/**
 *  @brief Switch position of Y and Z values
 *
 *  @param velocity pointer to velocity array
 */
void Kinematics::SwitchYZ() {
    int16_t tmp = 0;
    tmp = _velocities[transY];
    _velocities[transY] = _velocities[transZ];
    _velocities[transZ] = tmp;

    tmp = _velocities[rotY];
    _velocities[rotY] = _velocities[rotZ];
    _velocities[rotZ] = tmp;
}

/**
 *  @brief  Check if translation or rotation is dominant and set the other values to zero to allow exclusively rotation or translation
 *          to avoid issues with classics joysticks.
 *
 *  @param velocity pointer to velocity array
 */
void Kinematics::ExclusiveMode() {
    uint16_t totalRot = abs(_velocities[rotX]) + abs(_velocities[rotY]) + abs(_velocities[rotZ]);
    uint16_t totalTrans = abs(_velocities[transX]) + abs(_velocities[transY]) + abs(_velocities[transZ]);

    if (totalRot > totalTrans) {
        _velocities[transX] = 0;
        _velocities[transY] = 0;
        _velocities[transZ] = 0;
    } else {
        _velocities[rotX] = 0;
        _velocities[rotY] = 0;
        _velocities[rotZ] = 0;
    }
}

// TODO Can be retrieved from ENUM
#define INPUTCHECK_MODFUNC_MINVAL 0 // The lowest modulation function identifier
#define INPUTCHECK_MODFUNC_MAXVAL 4 // The highest modulation function identifier
/**
 * Modfunc commands send through the Serial Monitor should be in the format
 *
 *   3 00000 x              where x is the new modfunc value
 *
 */
void Kinematics::SetModulationFunction(uint8_t requestedModfunc) {

    if (requestedModfunc < INPUTCHECK_MODFUNC_MINVAL || INPUTCHECK_MODFUNC_MAXVAL > 4) {
        // We service four modifier functions, number 0..4
        Serial.println(F("Invalid modfunc request."));
    } else {

        // Write the value to the Serial Monitor for user feedback.
        Serial.print(F("Updated modifier function to: "));
        Serial.println(requestedModfunc);

        // Store the sensitivity in the EEPROM. EEPROM.put() uses EEPROM.update and thus only writes data if the data has changed.
        EEPROM.put(EEPROM_ADDRESS_MODFUNC, requestedModfunc);
        _modFunc = requestedModfunc;
    }
}

#define INPUTCHECK_INVERSION_MINVAL 0   // The lowest inversion value
#define INPUTCHECK_INVERSION_MAXVAL 1   // The highest inversion value
#define INPUTCHECK_INVERSION_MINCMD 400 // The lowest inversion command value
#define INPUTCHECK_INVERSION_MAXCMD 405 // The highest inversion command value
void Kinematics::SetTransRotInversions(uint8_t requestedVelocity, uint8_t requestedValue) {

    // Check if the value and the command are within our boundaries.
    if (requestedValue < INPUTCHECK_INVERSION_MINVAL || requestedValue > INPUTCHECK_INVERSION_MAXVAL || requestedVelocity < INPUTCHECK_INVERSION_MINCMD || requestedVelocity > INPUTCHECK_INVERSION_MAXCMD) {
        Serial.println(F("Invalid inversion command"));
    } else {

        Serial.print(F("Updating INV"));
        Serial.print(_velocityNames[requestedVelocity]);
        Serial.print(F(" to "));
        Serial.println(requestedValue);

        if (requestedValue) {
            // Set the commanded bit
            _inversions |= (1u << requestedVelocity);
        } else {
            // Unset the commamded bit
            _inversions &= ~(1u << requestedVelocity);
        }

        // Store the value in the EEPROM. EEPROM.put() uses EEPROM.update and thus only writes data if the data has changed.
        EEPROM.put(EEPROM_ADDRESS_INVERSION, _inversions);

        PrintTransRotInversions(true);
    }
}

/**
 * @brief Set the sensitivity for the specified axis.
 * @param axis The axis to set the sensitivity for.
 * @param isGT True if the axis is a GT axis, false otherwise.
 * @param pos_neg 1 for positive, -1 for negative, 0 for both.
 * @param value The new sensitivity value.
 */
void Kinematics::SetSensitivities(velocityAxis_t axis, boolean isGT, boolean isMF, int8_t pos_neg, float value) { // Set the sensitivity for the specified axis
    // Check if the axis is valid
    if (axis < 0 || axis >= velocityAxis_t::LENGTH) {
        Serial.println(F("Invalid axis."));
        return;
    }

    // Check if the value is valid
    if (_isNaN(value)) {
        Serial.println(F("Invalid value."));
        return;
    }

    if (isGT) {
        switch (pos_neg) {
        case 1:
            _sensitivities[axis].pos_gate = value;
            break;
        case -1:
            _sensitivities[axis].neg_gate = value;
            break;
        default:
            _sensitivities[axis].pos_gate = value; // Set both gates to the same value
            _sensitivities[axis].neg_gate = value;
            break;
        }
    } else if (isMF) {
        switch (pos_neg) {
        case 1:
            _sensitivities[axis].pos_modfunc = (int8_t)value;
            break;
        case -1:
            _sensitivities[axis].neg_modfunc = (int8_t)value;
            break;
        default:
            _sensitivities[axis].pos_modfunc = (int8_t)value; // Set both mod functions to the same value
            _sensitivities[axis].neg_modfunc = (int8_t)value;
            break;
        }
    } else {
        // If the axis is not a GT or MF axis, set the sensitivity value
        switch (pos_neg) {
        case 1:
            _sensitivities[axis].pos_sensitivity = value;
            break;
        case -1:
            _sensitivities[axis].neg_sensitivity = value;
            break;
        default:
            _sensitivities[axis].pos_sensitivity = value; // Set both sensitivities to the same value
            _sensitivities[axis].neg_sensitivity = value;
            break;
        }
    }

    // Store the sensitivity in the EEPROM. EEPROM.put() uses EEPROM.update and thus only writes data if the data has changed.
    EEPROM.put(EEPROM_ADDRESS_SENSITIVITIES + (axis * sizeof(sensitivity_t)), _sensitivities[axis]);
}

void Kinematics::PrintModulationFunction(bool introText) {
    if (introText) {
        Serial.println(F("Current modfunc: "));
    }

    Serial.print(_modFunc);

    if (introText) {
        Serial.print(DEBUG_LINE_END);
    }
}

void Kinematics::PrintTransRotInversions(bool introText) {
    if (introText) {
        Serial.println(F("Current trans/rot inversions: "));
    }

    Serial.print(F("{ "));

    char debugOutputBuffer[20];
    for (uint8_t i = 0; i < velocityAxis_t::LENGTH; i++) {
        sprintf(debugOutputBuffer, "INV %2.2s: %1d, ", _velocityNames[i], GET_INVERSION(_inversions, i));
        Serial.print(debugOutputBuffer);
    }

    Serial.print(F("}"));

    if (introText) {
        Serial.print(DEBUG_LINE_END);
    }
}

#define S_PRINTKOMMA(x) \
    Serial.print(x);    \
    Serial.print(F(", "))
void Kinematics::PrintSensitivities(bool introText) {
    // Print the current sensitivities to the Serial Monitor. The sensitivities are stored in the EEPROM and can be changed by the user.
    // The sensitivities are stored as a struct in the EEPROM.
    // Each VelocityAxis has a sensitivity and a gate value for the positive and the negative motion.
    if (introText) {
        Serial.println(F("Current sensitivities (200-210)(4 digit command) :"));
    }

    for (uint8_t i = 0; i < velocityAxis_t::LENGTH; i++) {
        if (i != 0) {
            Serial.print(F(", "));
        }

        // Print the sensitivity values for each velocity axis in the format     TX+: TX-: GTX+: GTX-: TXMF+: TXMF-:
        Serial.print(_velocityNames[i]);
        Serial.print(F("+: "));
        Serial.print(_sensitivities[i].pos_sensitivity);

        Serial.print(F(", "));
        Serial.print(_velocityNames[i]);
        Serial.print(F("-: "));
        Serial.print(_sensitivities[i].neg_sensitivity);

        if (_sensitivities[i].pos_gate != 0) {
            Serial.print(F(", "));
            Serial.print(_velocityNames[i]);
            Serial.print(F("G+: "));
            Serial.print(_sensitivities[i].pos_gate);
        }
        if (_sensitivities[i].neg_gate != 0) {
            Serial.print(F(", "));
            Serial.print(_velocityNames[i]);
            Serial.print(F("G-: "));
            Serial.print(_sensitivities[i].neg_gate);
        }
        if (_sensitivities[i].pos_modfunc != -1) {
            Serial.print(F(", "));
            Serial.print(_velocityNames[i]);
            Serial.print(F("MF+: "));
            Serial.print(_sensitivities[i].pos_modfunc);
        }
        if (_sensitivities[i].neg_modfunc != -1) {
            Serial.print(F(", "));
            Serial.print(_velocityNames[i]);
            Serial.print(F("MF-: "));
            Serial.print(_sensitivities[i].neg_modfunc);
        }
    }

    if (introText) {
        Serial.print(DEBUG_LINE_END);
    }
}

/**
 * @brief Setup the velocity parameters for the given velocity type.
 *        This function is used to setup the velocity parameters for the given velocity type.
 *
 * @param param The velocity parameters to setup.
 * @param pos_sensitivity The positive sensitivity to set.
 * @param neg_sensitivity The negative sensitivity to set.
 * @param pos_gate The positive gate to set.
 * @param neg_gate The negative gate to set.
 * @param pos_modfunc The positive modulation function to set.
 * @param neg_modfunc The negative modulation function to set.
 * @return void
 */
void Kinematics::__setupSensitivity(sensitivity_t *param, float pos_sensitivity, float neg_sensitivity, int8_t pos_gate, int8_t neg_gate, int8_t pos_modfunc, int8_t neg_modfunc) {
    param->pos_sensitivity = pos_sensitivity;
    param->neg_sensitivity = neg_sensitivity;
    param->pos_modfunc = pos_modfunc;
    param->pos_gate = pos_gate;
    param->neg_gate = neg_gate;
    param->neg_modfunc = neg_modfunc;
}

void Kinematics::_getDefaultSensitivities(sensitivity_t *defparams) {
    __setupSensitivity(&defparams[transX], DEF_TRANSX, DEF_TRANSX, 0, 0, -1, -1);
    __setupSensitivity(&defparams[transY], DEF_TRANSY, DEF_TRANSY, 0, 0, -1, -1);
    __setupSensitivity(&defparams[transZ], DEF_POSITIVE_TRANSZ, DEF_NEGATIVE_TRANSZ, 0, DEF_GATE_NEG_TRANSZ, 0, -1); // The positive transZ has a linear scaling modifuer function override.
    __setupSensitivity(&defparams[rotX], DEF_ROTX_SENSITIVITY, DEF_ROTX_SENSITIVITY, DEF_GATE_ROTX, DEF_GATE_ROTX, -1, -1);
    __setupSensitivity(&defparams[rotY], DEF_ROTY_SENSITIVITY, DEF_ROTY_SENSITIVITY, DEF_GATE_ROTY, DEF_GATE_ROTY, -1, -1);
    __setupSensitivity(&defparams[rotZ], DEF_ROTZ_SENSITIVITY, DEF_ROTZ_SENSITIVITY, DEF_GATE_ROTZ, DEF_GATE_ROTZ, -1, -1);
}

void Kinematics::_loadVelocitySensitivities() {
    // Setup the default values for the velocity parameters.
    sensitivity_t nonEEPROMDefaults[velocityAxis_t::LENGTH] = {};
    _getDefaultSensitivities(nonEEPROMDefaults);

    for (int i = 0; i < (velocityAxis_t::LENGTH); i++) {
        // Read a velocity parameter from the EEPROM, the values are stored as a struct velocity_Parameter_t.
        EEPROM.get(EEPROM_ADDRESS_SENSITIVITIES + (i * sizeof(sensitivity_t)), _sensitivities[i]);

        // Check if the value is set by checking the sensitivity oart of the struct.
        // If not, set it to the default value and store that value in the EEPROM.
        if (_isNaN(_sensitivities[i].pos_sensitivity) || FORCE_EEPROM_INIT) {
            // The EEPROM doesn't have correct data. Use our defaults.
            _sensitivities[i] = nonEEPROMDefaults[i];

            // Store the default value in the EEPROM.
            EEPROM.put(EEPROM_ADDRESS_SENSITIVITIES + (i * sizeof(sensitivity_t)), nonEEPROMDefaults[i]);
        }
    }
}