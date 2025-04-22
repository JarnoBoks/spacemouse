// This file contains all functions to calculate the kinematics

// The user specific settings, like pin mappings or special configuration variables and sensitivities are stored in config.h.
// Please open config_sample.h, adjust your settings and save it as config.h

#include <Arduino.h>
#include "config.h"        // Include the config file for the hardware and the kinematics
#include "kinematics.h"    // Our header file for the kinematics
#include "eepromStorage.h" // Include the EEPROM address map
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
#if 0
#define NO_INVERT 0
#define INVERT 1
#define NO_GATE 0
#define SQUARED_TAN 3
#define LINEAR 0
#endif

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

void Kinematics::_setDefaultAxisConfigurations() {
    _AxesConfigurations[transX] = new MotionAxisConfig("TX", EEPROM_ADDRESS_CFG_TX, VelocityConfig_t(DEF_SENS_TX_POS, DEF_SENS_TX_NEG, DEF_GATE_TX_POS, DEF_GATE_TX_NEG, DEF_MF_TX_POS, DEF_MF_TX_NEG, DEF_INVERT_TX));
    _AxesConfigurations[transY] = new MotionAxisConfig("TY", EEPROM_ADDRESS_CFG_TY, VelocityConfig_t(DEF_SENS_TY_POS, DEF_SENS_TY_NEG, DEF_GATE_TY_POS, DEF_GATE_TY_NEG, DEF_MF_TY_POS, DEF_MF_TY_NEG, DEF_INVERT_TY));
    _AxesConfigurations[transZ] = new MotionAxisConfig("TZ", EEPROM_ADDRESS_CFG_TZ, VelocityConfig_t(DEF_SENS_TZ_POS, DEF_SENS_TZ_NEG, DEF_GATE_TZ_POS, DEF_GATE_TZ_NEG, DEF_MF_TZ_POS, DEF_MF_TZ_NEG, DEF_INVERT_TZ));
    _AxesConfigurations[rotX] = new MotionAxisConfig("RX", EEPROM_ADDRESS_CFG_RX, VelocityConfig_t(DEF_SENS_RX_POS, DEF_SENS_RX_NEG, DEF_GATE_RX_POS, DEF_GATE_RX_NEG, DEF_MF_RX_POS, DEF_MF_RX_NEG, DEF_INVERT_RX));
    _AxesConfigurations[rotY] = new MotionAxisConfig("RY", EEPROM_ADDRESS_CFG_RY, VelocityConfig_t(DEF_SENS_RY_POS, DEF_SENS_RY_NEG, DEF_GATE_RY_POS, DEF_GATE_RY_NEG, DEF_MF_RY_POS, DEF_MF_RY_NEG, DEF_INVERT_RY));
    _AxesConfigurations[rotZ] = new MotionAxisConfig("RZ", EEPROM_ADDRESS_CFG_RZ, VelocityConfig_t(DEF_SENS_RZ_POS, DEF_SENS_RZ_NEG, DEF_GATE_RZ_POS, DEF_GATE_RZ_NEG, DEF_MF_RZ_POS, DEF_MF_RZ_NEG, DEF_INVERT_RZ));
}

void Kinematics::_loadAxisConfigurations() {
    _AxesConfigurations[transX] = new MotionAxisConfig("TX", EEPROM_ADDRESS_CFG_TX);
    _AxesConfigurations[transY] = new MotionAxisConfig("TY", EEPROM_ADDRESS_CFG_TY);
    _AxesConfigurations[transZ] = new MotionAxisConfig("TZ", EEPROM_ADDRESS_CFG_TZ);
    _AxesConfigurations[rotX] = new MotionAxisConfig("RX", EEPROM_ADDRESS_CFG_RX);
    _AxesConfigurations[rotY] = new MotionAxisConfig("RY", EEPROM_ADDRESS_CFG_RY);
    _AxesConfigurations[rotZ] = new MotionAxisConfig("RZ", EEPROM_ADDRESS_CFG_RZ);
}

/**
 *  @brief Calculates the kinematics of the six axis (TX, TY, TZ, RX, RY, RZ) from the eight sensors
 */
void Kinematics::CalculcateKinematic() {

    // Get raw kinematics from the hardwarwe sensors. The raw kinematics are calculated in the hardware class (due to the hardware specific implementation).
    int16_t rawSensorVelocities[enumAxis_t::LENGTH];
    _SMHW->CalculateKinematicSensors(rawSensorVelocities);

    for (uint8_t axs = 0; axs != enumAxis_t::LENGTH; axs++) {
        _AxesConfigurations[axs]->CalculcateVelocity(rawSensorVelocities[axs]);
        _velocities[axs] = _AxesConfigurations[axs]->GetVelocity(); // FIXME - We have a local storage of velocities for now
    }
}

void Kinematics::PrintVelocities() {
    for (int i = 0; i < enumAxis_t::LENGTH; i++) {
        _AxesConfigurations[i]->PrintVelocity();
    }
}

int16_t Kinematics::GetVelocity(const enumAxis_t axis) {
    return _AxesConfigurations[axis]->GetVelocity();
}

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
 *  @brief Switches position of Y and Z values for both translation and rotation, if configured in the EEPROM.
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
 *  @brief Prevents conflicts between translation and rotation movements, if configured in the EEPROM. Keeps only the dominant movement type.
 *  @details This function ensures that only one type of movement is active at a time,
 *           preventing conflicts between translation and rotation movements.
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
 * @return 1 if the configuration was updated successfully, -1 if the axis name is invalid.
 */
int8_t Kinematics::UpdateAxisConfig(const char *axisName, const boolean isGT, const boolean isMF, const boolean isInversion, const int8_t pos_neg, const float value) { // Set the sensitivity for the specified axis

    // Loop over the axis names and check if the axis name is valid.
    // If the axis name is valid, update the configuration for the specified axis.
    enumAxis_t axis = enumAxis_t::LENGTH;
    for (uint8_t i = 0; i < enumAxis_t::LENGTH; i++) {
        if (_AxesConfigurations[i]->UpdateConfig(axisName, isGT, isMF, isInversion, pos_neg, value)) {
            return 1;
        }
    };

    if (axis == enumAxis_t::LENGTH) {
        Serial.println(CF(Error_InvalidAxisName));
        return -1;
    }

    return 0; // Success
}

int8_t Kinematics::UpdateSwitchYZ(const bool switchOn) { // Update the switch YZ configuration
    _switchYZ = switchOn;                                // Update the internal switch YZ flag
    EEPROM.put(EEPROM_ADDRESS_SWITCHYZ, _switchYZ);      // Store the switch YZ value in the EEPROM
    return 1;                                            // Indicate success
}

void Kinematics::PrintSwitchYZ() {
    Serial.print(F("Switch YZ: "));
    Serial.println(_switchYZ);
}

boolean Kinematics::GetAxisInvert(const enumAxis_t axis) {
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
 * @brief Calculate which velocity is the main action. What is the strongest movement?
 * @return index with the biggest velocity. returns -1 if all in deadzone
 */
enumAxis_t Kinematics::GetMainVelocity() {
    int8_t mainVelocity = -1;
    int16_t velMax = 0;
    for (int i = 0; i < enumAxis_t::LENGTH; i++) {
        // bigger than deadzone and bigger than before?
        if ((abs(_velocities[i]) > velMax) && (abs(_velocities[i]) > VELOCITY_DEADZONE_FOR_LED)) {
            velMax = abs(_velocities[i]);
            mainVelocity = i;
        }
    }
    return static_cast<enumAxis_t>(mainVelocity);
}

void Kinematics::setIgnoreArithmetics(const bool ignore) {
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

void Kinematics::UpdateExclusiveMode(const bool exclusiveMode) {
    _exclusiveMode = exclusiveMode;                       // Update the exclusive mode configuration
    EEPROM.put(EEPROM_ADDRESS_EXCLUSIVE, _exclusiveMode); // Store the exclusive mode value in the EEPROM
}
void Kinematics::PrintExclusiveMode() { // Print the exclusive mode configuration
    Serial.print(F("Exclusive Mode: "));
    Serial.print(_exclusiveMode);
}
