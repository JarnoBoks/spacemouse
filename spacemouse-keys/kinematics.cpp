// This file contains all functions to calculate the kinematics

// The user specific settings, like pin mappings or special configuration variables and sensitivities are stored in config.h.
// Please open config_sample.h, adjust your settings and save it as config.h

#include <Arduino.h>
#include "config.h"                         // Include the config file for the hardware and the kinematics
#include "..\src\kinematics\Kinematics.hpp" // Our header file for the kinematics
#include "eepromStorage.h"                  // Include the EEPROM address map
#include <EEPROM.h>
#include "text.h" // Include the text file for the debug output

#ifdef HALLEFFECT
#include "hardware/SpaceMouseHW_Hall.h"
#else
#include "hardware/SpaceMouseHW_Joystick.h"
#endif

// Default value for the switch YZ configuration - do not switch the YZ axis. The default can be overiden in the config.h file.
// The default value is used (and stored) if the EEPROM is not set yet.
#ifndef SWITCHYZ
#define SWITCHYZ 0 // Switch Zoom direction with Up/Down Movement
#endif

// Preprocessor directive to check if the exclusive mode is enabled. Necessary to keep the legacy config.h definitions working.
#ifdef EXCLUSIVEMODE
#define EXCLUSIVESETTING 1
#else
#define EXCLUSIVESETTING 0
#endif

/**
 * @brief Constructor for Kinematics class.
 * @details Create the object & technically initialise members.Load the configuration data from the EEPROM.
 *
 * @param firstrun If this is the first run, we need to set the default values for the velocities. The defaults are configured in
 *                 hardware/SpaceMouseHW_(Hall/Joystick).h. If it is not the first run, we need to load the configuration from the EEPROM.
 */
Kinematics::Kinematics(SpaceMouseHW_ &Mouse_Hardware) : _SMHW(&Mouse_Hardware) {
    // Initialize velocities
    for (uint8_t idx = transX; idx != enumAxis_t::LENGTH; idx++) {
        _velocities[idx] = 0;
    }

    // Read default configurations from EEPROM, use the default configured value if the byte is not set.
    if (EEPROMStorage::isFirstRun()) {
        // If this is the first run, we need to set the default values for the velocities.
        // The default values are defined in config.h and are used if the EEPROM is not set yet.
        _setDefaultAxisConfigurations();                        // Set the default axis configurations
        EEPROM.put(EEPROM_ADDRESS_SWITCHYZ, SWITCHYZ);          // Store the switchYZ value in the EEPROM
        EEPROM.put(EEPROM_ADDRESS_EXCLUSIVE, EXCLUSIVESETTING); // Store the exclusive mode value in the EEPROM

    } else {
        _loadAxisConfigurations();                            // Load the Axis configuration from the EEPROM
        EEPROM.get(EEPROM_ADDRESS_SWITCHYZ, _switchYZ);       // Load the switch YZ configuration from the EEPROM
        EEPROM.get(EEPROM_ADDRESS_EXCLUSIVE, _exclusiveMode); // Load the exclusive mode configuration from the EEPROM
    }
}

/**
 * @brief Destructor for Kinematics class.
 * @details Destroy the object and free the memory allocated for the MotionAxisConfig objects.
 *          The destructor is called when the object is destroyed.
 */
Kinematics::~Kinematics() {
#ifdef ARDUINO_ARCH_ESP32
    // Destroy the MotionAxisConfig objects to free memory.
    for (uint8_t idx = transX; idx != enumAxis_t::LENGTH; idx++) {
        delete _AxisConfigurations[idx];
    }
#endif
};

/**
 * @brief  Restores default configurations for the axes.
 */
void Kinematics::RestoreDefaultAxisConfigurations() {
    for (uint8_t idx = transX; idx != enumAxis_t::LENGTH; idx++) {
        delete _AxesConfigurations[idx]; // Delete the old configuration
    }
    _setDefaultAxisConfigurations(); // Set the default axis configurations
}

/**
 * @brief Creates the 6 axes with the default configuration parameters
 */
void Kinematics::_setDefaultAxisConfigurations() {
    _AxesConfigurations[transX] = new MotionAxisConfig("TX", EEPROM_ADDRESS_CFG_TX, VelocityConfig_t(DEF_SENS_TX_POS, DEF_SENS_TX_NEG, DEF_GATE_TX_POS, DEF_GATE_TX_NEG, DEF_MF_TX_POS, DEF_MF_TX_NEG, DEF_INVERT_TX));
    _AxesConfigurations[transY] = new MotionAxisConfig("TY", EEPROM_ADDRESS_CFG_TY, VelocityConfig_t(DEF_SENS_TY_POS, DEF_SENS_TY_NEG, DEF_GATE_TY_POS, DEF_GATE_TY_NEG, DEF_MF_TY_POS, DEF_MF_TY_NEG, DEF_INVERT_TY));
    _AxesConfigurations[transZ] = new MotionAxisConfig("TZ", EEPROM_ADDRESS_CFG_TZ, VelocityConfig_t(DEF_SENS_TZ_POS, DEF_SENS_TZ_NEG, DEF_GATE_TZ_POS, DEF_GATE_TZ_NEG, DEF_MF_TZ_POS, DEF_MF_TZ_NEG, DEF_INVERT_TZ));
    _AxesConfigurations[rotX] = new MotionAxisConfig("RX", EEPROM_ADDRESS_CFG_RX, VelocityConfig_t(DEF_SENS_RX_POS, DEF_SENS_RX_NEG, DEF_GATE_RX_POS, DEF_GATE_RX_NEG, DEF_MF_RX_POS, DEF_MF_RX_NEG, DEF_INVERT_RX));
    _AxesConfigurations[rotY] = new MotionAxisConfig("RY", EEPROM_ADDRESS_CFG_RY, VelocityConfig_t(DEF_SENS_RY_POS, DEF_SENS_RY_NEG, DEF_GATE_RY_POS, DEF_GATE_RY_NEG, DEF_MF_RY_POS, DEF_MF_RY_NEG, DEF_INVERT_RY));
    _AxesConfigurations[rotZ] = new MotionAxisConfig("RZ", EEPROM_ADDRESS_CFG_RZ, VelocityConfig_t(DEF_SENS_RZ_POS, DEF_SENS_RZ_NEG, DEF_GATE_RZ_POS, DEF_GATE_RZ_NEG, DEF_MF_RZ_POS, DEF_MF_RZ_NEG, DEF_INVERT_RZ));
}

/**
 * @brief Creates the 6 axes with the configuration paramaters loaded from the EEPROM.
 */
void Kinematics::_loadAxisConfigurations() {
    _AxesConfigurations[transX] = new MotionAxisConfig("TX", EEPROM_ADDRESS_CFG_TX);
    _AxesConfigurations[transY] = new MotionAxisConfig("TY", EEPROM_ADDRESS_CFG_TY);
    _AxesConfigurations[transZ] = new MotionAxisConfig("TZ", EEPROM_ADDRESS_CFG_TZ);
    _AxesConfigurations[rotX] = new MotionAxisConfig("RX", EEPROM_ADDRESS_CFG_RX);
    _AxesConfigurations[rotY] = new MotionAxisConfig("RY", EEPROM_ADDRESS_CFG_RY);
    _AxesConfigurations[rotZ] = new MotionAxisConfig("RZ", EEPROM_ADDRESS_CFG_RZ);
}

/**
 *  @brief Calculates the kinematics of the six axes (TX, TY, TZ, RX, RY, RZ) from the eight sensors
 *
 * Each axis kinematics are recalculated, with the updated velocities based on the raw sensor kinematics.
 * Each axis will apply sensitivity, gate, modifier function and inversion to the raw sensor kinematics.
 */
void Kinematics::CalculcateKinematic() {

    // Get raw kinematics from the hardware sensors. The raw kinematics are calculated in the hardware class (due to the hardware specific implementation).
    int16_t rawSensorVelocities[enumAxis_t::LENGTH];
    _SMHW->CalculateKinematicSensors(rawSensorVelocities);

    for (uint8_t axs = 0; axs != enumAxis_t::LENGTH; axs++) {
        _AxesConfigurations[axs]->CalculcateVelocity(rawSensorVelocities[axs]);

        // REVIEW - For now a local copy of the velocities. Should be replaced by a function that requests velocity from the axis configurations.
        _velocities[axs] = _AxesConfigurations[axs]->GetVelocity();
    }
}

/**
 * @brief Reports the current velocity for each axis to the serial monitor.
 */
void Kinematics::PrintVelocities() {
    for (int i = 0; i < enumAxis_t::LENGTH; i++) {
        _AxesConfigurations[i]->PrintVelocity();
    }
}

/**
 * @brief Retrieves the velocity for the specified axis.
 * @details This function returns the velocity for the specified axis.
 * @param axis The axis for which the velocity is retrieved.
 * @return The velocity of the specified axis.
 */
int16_t Kinematics::GetVelocity(const enumAxis_t axis) {
    return _AxesConfigurations[axis]->GetVelocity();
}

/**
 * @brief Sets the velocity for the specified axis.
 * @details This function sets the velocity for the specified axis.
 * @param axis The axis for which the velocity is set.
 * @param velocity The velocity to set for the specified axis.
 */
void Kinematics::SetVelocity(const enumAxis_t axis, const int16_t velocity) {
    _AxesConfigurations[axis]->SetVelocity(velocity);
}

#if 0
// JB - I think this function isn't used anywhere
/**
 *  @brief Switches position of X and Y values for both translation and rotation
 */
void Kinematics::SwitchXY() {
    if (_switchXY) {
        int16_t tmp = 0;
        tmp = _velocities[transX];
        _velocities[transX] = _velocities[transY];
        _velocities[transY] = tmp;

        tmp = _velocities[rotX];
        _velocities[rotX] = _velocities[rotY];
        _velocities[rotY] = tmp;
    }
}
#endif

/**
 *  @brief Switches position of trans Y and trans Z values for both translation and rotation, if configured in the EEPROM.
 *  If the member _ignoreArithmetic is set, the switch is not performed.
 */
void Kinematics::ProcessSwitchYZ() {
    if (_switchYZ && !_ignoreArithmetic) {
        int16_t tmp = 0;
        tmp = _velocities[transY];
        _velocities[transY] = _velocities[transZ];
        _velocities[transZ] = tmp;

        tmp = _velocities[rotY];
        _velocities[rotY] = _velocities[rotZ];
        _velocities[rotZ] = tmp;
    }
}

/**
 *  @brief Processes the Exclusive mode on the current velocities if configured in the EEPROM.
 *  @details Prevents conflicts between translation and rotation movements. Keeps only the dominant movement type.
 *  This function ensures that only one type of movement is active at a time, preventing conflicts between translation and rotation movements.
 *  If the member _ignoreArithmetic is set, the exclusive mode is not applied.
 */
void Kinematics::ProcessExclusiveMode() {
    if (_exclusiveMode && !_ignoreArithmetic) {
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
}

/**
 * @brief Updates the configuration for the specified axis.
 * @param axisName The name of the axis (TX,TY,TZ,RX,RY,RZ) to set the configuration for.
 * @param isGT True if value configures a gate , false otherwise.
 * @param isMF True if value configures a modifier function, false otherwise.
 * @param isInversion True if the value configures an inversion, false otherwise.
 * @param pos_neg   1 configures the value for a positive direction,
 *                 -1 configures the value for a negative direction,
 *                  0 configures the value for both directions.
 * @param value The configuration value.
 *
 * @return Indicates success or failure of the update.
 * @retval 1 Update was successful.
 * @retval 0 Update was not successful.
 */
uint8_t Kinematics::UpdateAxisConfiguration(const char *axisName, const boolean isGT, const boolean isMF, const boolean isInversion, const int8_t pos_neg, const float value) { // Set the sensitivity for the specified axis

    // Dispatches the update to all axes. The axis name is checked in the MotionAxisConfig class.
    // The function returns true if the axis name is valid and the configuration was updated in the EEPROM successfully.
    enumAxis_t axis = enumAxis_t::LENGTH;
    for (uint8_t i = 0; i < enumAxis_t::LENGTH; i++) {
        if (_AxesConfigurations[i]->UpdateConfig(axisName, isGT, isMF, isInversion, pos_neg, value)) {
            return 1;
        }
    };

    // If the function reaches here, the axis name was not valid.
    // Print an error message to the serial monitor.
    if (axis == enumAxis_t::LENGTH) {
        Serial.println(CF(Error_InvalidAxisName));
        return 0;
    }

    return 0; // Failure
}

/**
 * @brief Updates the switch YZ configuration in the EEPROM and sets the internal flag.
 * @param switchyz_on True to switch the YZ axes, false to keep them in their original position.
 * @return Indicates success or failure of the update.
 * @retval 1 Update was successful.
 * @retval 0 Update was not successful.
 */
int8_t Kinematics::UpdateSwitchYZ(const bool switchyz_on) { // Update the switch YZ configuration
    _switchYZ = switchyz_on;                                // Update the internal switch YZ flag
    EEPROM.put(EEPROM_ADDRESS_SWITCHYZ, _switchYZ);         // Store the switch YZ value in the EEPROM
    return 1;                                               // Indicate success
}

/**
 * @brief Writes the switch YZ configuration to the serial monitor.
 * @details This function prints the current switch YZ configuration to the serial monitor.
 */
void Kinematics::PrintSwitchYZ() {
    Serial.print(F("Switch YZ: "));
    Serial.println(_switchYZ);
}

/**
 * @brief Retrieves the inversion configuration for the specified axis.
 * @param axis The axis for which to retrieve the inversion configuration.
 * @return True if the axis is inverted, false otherwise.
 * @see Ledring.cpp for usage
 */
boolean Kinematics::GetAxisInversion(const enumAxis_t axis) {
    // Check if the axis is valid and return the inversion for the specified axis.
    if (axis < enumAxis_t::LENGTH) {
        return (_AxesConfigurations[axis]->GetInvert() == 1);
    } else {
        Serial.println(CF(Error_InvalidAxisName));
        return false;
    }
}

/**
 * @brief Outputs the configuration of all axes to the Serial monitor.
 */
void Kinematics::PrintAxisConfigurations() {
    // Print the header for the axis configuration
    Serial.println(F("\nSensitivity          | Gate               | ModFunc          | Invert"));
    for (uint8_t i = 0; i < enumAxis_t::LENGTH; i++) {
        // Print the sensitivity values for each velocity axis
        _AxesConfigurations[i]->PrintConfig();
        Serial.print(DEBUG_LINE_END);
    }
}

/**
 * @brief Calculates which velocity is the main action. What is the strongest movement?
 * @return index with the biggest velocity. Returns enumAxis_t::LENGTH if all in deadzone
 * @see Ledring.cpp for usage
 */
enumAxis_t Kinematics::GetMainVelocity() {
    int8_t mainVelocity = -1;
    int16_t velMax = 0;
    for (int i = 0; i < enumAxis_t::LENGTH; i++) {
        // bigger than deadzone and bigger than before?
        if ((abs(_velocities[i]) > velMax) && (abs(_velocities[i]) > VELOCITYDEADZONEFORLED)) {
            velMax = abs(_velocities[i]);
            mainVelocity = i;
        }
    }

    return static_cast<enumAxis_t>(mainVelocity);
}

/**
 * @brief Sets the ignore arithmetic flag for the kinematics calculations.
 * If this flag is set, the arithmetic calculations (modifier function, inversion, switchYZ and exclusive mode) are ignored.
 * @param ignore True to ignore arithmetic calculations, false to enable them.
 */
void Kinematics::SetIgnoreArithmetics(const bool ignore) {
    if (ignore) {
        _ignoreArithmetic = true;
        for (uint8_t i = 0; i < enumAxis_t::LENGTH; i++) {
            _AxesConfigurations[i]->SetModFunc(ModFunc_t::mfLINEAR); // Temporary set the modulation function to linear
            _AxesConfigurations[i]->SetInvert(0);                    // Temporary set the inversion to 0 (no inversion)
        }
    } else if (_ignoreArithmetic) {
        _ignoreArithmetic = false; // Reset the ignore flag
        _loadAxisConfigurations(); // Load the axis configurations from the EEPROM
    }
}

/**
 * @brief Updates the exclusive mode configuration in the EEPROM and sets the internal flag.
 * @param exclusivemode_on True to enable exclusive mode, false to disable it.
 */
void Kinematics::UpdateExclusiveMode(const bool exclusivemode_on) {
    _exclusiveMode = exclusivemode_on;                    // Update the exclusive mode configuration
    EEPROM.put(EEPROM_ADDRESS_EXCLUSIVE, _exclusiveMode); // Store the exclusive mode value in the EEPROM
}

/**
 * @brief Prints the exclusive mode configuration to the serial monitor.
 * @details This function outputs the current exclusive mode configuration to the serial monitor.
 */
void Kinematics::PrintExclusiveMode() { // Print the exclusive mode configuration
    Serial.print(F("Exclusive Mode: "));
    Serial.println(_exclusiveMode);
}
