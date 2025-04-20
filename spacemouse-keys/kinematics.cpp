// This file contains all functions to calculate the kinematics

// The user specific settings, like pin mappings or special configuration variables and sensitivities are stored in config.h.
// Please open config_sample.h, adjust your settings and save it as config.h

#include <Arduino.h>
#include "config.h"        // Include the config file for the hardware and the kinematics
#include "kinematics.h"    // Our header file for the kinematics
#include "eepromStorage.h" // Include the EEPROM address map
#include "text.h"          // Include the text file for the debug output

#ifdef HALLEFFECT
#include "hardware/SpaceMouseHW_Hall.h"
#else
#include "hardware/SpaceMouseHW_Joystick.h"
#endif

/**
 * @brief Constructor for Kinematics class.
 * @details Create the object & technically initialise members.Load the configuration data from the EEPROM.
 *
 * @param firstrun If this is the first run, we need to set the default values for the velocities. The defaults are configured in
 *                 hardware/SpaceMouseHW_(Hall/Joystick).h. If it is not the first run, we need to load the configuration from the EEPROM.
 */
#define NO_INVERT 1
#define INVERT -1
#define NO_GATE 0
#define SQUARED_TAN 3
#define LINEAR 0
Kinematics::Kinematics(SpaceMouseHW_ &Mouse_Hardware) : _SMHW(&Mouse_Hardware) {
    // Initialize velocities
    for (uint8_t idx = transX; idx != enumAxis_t::LENGTH; idx++) {
        _velocities[idx] = 0;
    }

    // Read modFunc from EEPROM, use the default configured value if the byte is not set.
    if (EEPROMStorage::isFirstRun()) {
        // If this is the first run, we need to set the default values for the velocities.
        // The default values are defined in config.h and are used if the EEPROM is not set yet.
        _AxesConfigurations[transX] = new MotionAxisConfig("TX", EEPROM_ADDRESS_CFG_TX, VelocityConfig_t(DEF_SENS_TRANSX, NO_GATE, SQUARED_TAN, DEF_INVERT_TX));
        _AxesConfigurations[transY] = new MotionAxisConfig("TY", EEPROM_ADDRESS_CFG_TY, VelocityConfig_t(DEF_SENS_TRANSY, NO_GATE, SQUARED_TAN, DEF_INVERT_TY));
        _AxesConfigurations[transZ] = new MotionAxisConfig("TZ", EEPROM_ADDRESS_CFG_TZ, VelocityConfig_t(DEF_SENS_POSITIVE_TRANSZ, DEF_SENS_NEGATIVE_TRANSZ, NO_GATE, DEF_GATE_NEG_TRANSZ, LINEAR, SQUARED_TAN, DEF_INVERT_TZ));
        _AxesConfigurations[rotX] = new MotionAxisConfig("RX", EEPROM_ADDRESS_CFG_RX, VelocityConfig_t(DEF_SENS_ROTX, DEF_GATE_ROTX, SQUARED_TAN, DEF_INVERT_RX));
        _AxesConfigurations[rotY] = new MotionAxisConfig("RY", EEPROM_ADDRESS_CFG_RY, VelocityConfig_t(DEF_SENS_ROTY, DEF_GATE_ROTY, SQUARED_TAN, DEF_INVERT_RY));
        _AxesConfigurations[rotZ] = new MotionAxisConfig("RZ", EEPROM_ADDRESS_CFG_RZ, VelocityConfig_t(DEF_SENS_ROTZ, DEF_GATE_ROTZ, SQUARED_TAN, DEF_INVERT_RZ));
    } else {
        _AxesConfigurations[transX] = new MotionAxisConfig("TX", EEPROM_ADDRESS_CFG_TX);
        _AxesConfigurations[transY] = new MotionAxisConfig("TY", EEPROM_ADDRESS_CFG_TY);
        _AxesConfigurations[transZ] = new MotionAxisConfig("TZ", EEPROM_ADDRESS_CFG_TZ);
        _AxesConfigurations[rotX] = new MotionAxisConfig("RX", EEPROM_ADDRESS_CFG_RX);
        _AxesConfigurations[rotY] = new MotionAxisConfig("RY", EEPROM_ADDRESS_CFG_RY);
        _AxesConfigurations[rotZ] = new MotionAxisConfig("RZ", EEPROM_ADDRESS_CFG_RZ);
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
 *  @brief Calculate the kinematic of the three axis from the eight sensors
 *  @param centered pointer to the array containing the eight centered values from the axis of the 4 joysticks or the 8 Hall Effect sensors
 *  @param velocity pointer to the result array that will contain the translational and rotational motions
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

int16_t Kinematics::GetVelocity(enumAxis_t axis) {
    return _AxesConfigurations[axis]->GetVelocity();
}

void Kinematics::SetVelocity(enumAxis_t axis, int16_t velocity) {
    _AxesConfigurations[axis]->SetVelocity(velocity);
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

/**
 * @brief Update the configuration for the specified axis.
 * @param axisName The name of the axis (TX,TY,TZ,RX,RY,RZ) to set the configuration for.
 * @param isGT True if value configures a gate , false otherwise.
 * @param isMF True if value configures a modulation function, false otherwise.
 * @param isInversion True if the value configures an inversion, false otherwise.
 * @param pos_neg   1 configures the value for a positive direction,
 *                 -1 configures the value for a negative direction,
 *                  0 configures the value for both directions.
 * @param value The configuration value.
 *
 * @return 1 if the configuration was updated successfully, -1 if the axis name is invalid.
 */
int8_t Kinematics::UpdateAxisConfig(const char *axisName, boolean isGT, boolean isMF, boolean isInversion, int8_t pos_neg, float value) { // Set the sensitivity for the specified axis

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

boolean Kinematics::GetAxisInvert(enumAxis_t axis) {
    // Check if the axis is valid and return the inversion for the specified axis.
    if (axis < enumAxis_t::LENGTH) {
        return (_AxesConfigurations[axis]->GetInvert() == 1);
    } else {
        Serial.println(CF(Error_InvalidAxisName));
        return false;
    }
}

/**
 * @brief Output the configuration of all axis to the Serial Monitor.
 */
void Kinematics::PrintAxisConfigurations() {
    Serial.println(F("Axis configurations: (IFxx: [Normal: 1, Invert: -1])")); // 68 bytes

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