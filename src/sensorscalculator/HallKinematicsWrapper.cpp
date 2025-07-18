/**
 * @file HallKinematicsWrapper.cpp
 * @brief C-style wrapper for HallSensorKinematics class integration
 *
 * This file provides C-style functions to integrate the HallSensorKinematics
 * class with the existing Arduino-style codebase.
 *
 * @author GitHub Copilot
 */

#include "HallSensorKinematics.hpp"

// Include config defaults if available
#ifdef DEF_SENS_TX_POS
// Config is available, use it
#else
// Define fallback defaults
#define DEF_SENS_TX_POS 0.8
#define DEF_SENS_TY_POS 0.99
#define DEF_SENS_TZ_POS 2.5
#define DEF_SENS_RX_POS 1.0
#define DEF_SENS_RY_POS 1.2
#define DEF_SENS_RZ_POS 0.9
#endif

// Global instance pointer
static HallSensorKinematics *g_hallKinematics = nullptr;

extern "C" {

HallSensorKinematics *hallKinematics_init() {
    if (g_hallKinematics == nullptr) {
        g_hallKinematics = new HallSensorKinematics();
    }
    return g_hallKinematics;
}

void hallKinematics_calculate(HallSensorKinematics *instance, int sensorReadings[8], int16_t motionVectors[6]) {
    if (instance != nullptr) {
        instance->calculateMotion(sensorReadings, motionVectors);
    }
}

void hallKinematics_setCalibration(HallSensorKinematics *instance, int zeroes[8], int mins[8], int maxs[8]) {
    if (instance != nullptr) {
        instance->setCalibration(zeroes, mins, maxs);
    }
}

void hallKinematics_setSensitivity(HallSensorKinematics *instance, int axis, float sensitivity) {
    if (instance != nullptr) {
        instance->setSensitivity(axis, sensitivity);
    }
}

void hallKinematics_destroy(HallSensorKinematics *instance) {
    if (instance != nullptr) {
        delete instance;
        if (instance == g_hallKinematics) {
            g_hallKinematics = nullptr;
        }
    }
}

} // extern "C"

/**
 * @brief Simple integration function for existing calculateKinematic style
 *
 * This function can be used as a drop-in replacement for the existing
 * calculateKinematic function in the spacemouse codebase.
 *
 * @param centered Array of 8 centered sensor readings
 * @param velocity Output array of 6 motion vectors
 */
void calculateHallKinematics(int *centered, int16_t *velocity) {
    // Initialize kinematics if not done already
    if (g_hallKinematics == nullptr) {
        g_hallKinematics = hallKinematics_init();
    }

    // Calculate motion using the advanced algorithm
    g_hallKinematics->calculateMotion(centered, velocity);
}

/**
 * @brief Set calibration from existing config arrays
 *
 * This function allows easy integration with existing calibration systems
 * by accepting the standard calibration arrays used in the spacemouse code.
 */
void setHallCalibrationFromConfig() {
    if (g_hallKinematics == nullptr) {
        g_hallKinematics = hallKinematics_init();
    }

    // Use default values from defaults_hall.h or fallbacks
    int defaultZeroes[8] = ELECTRICZEROES;
    int defaultMins[8] = MINVALS;
    int defaultMaxs[8] = MAXVALS;

    g_hallKinematics->setCalibration(defaultZeroes, defaultMins, defaultMaxs);
}

/**
 * @brief Apply sensitivity settings from configuration
 *
 * This function applies the sensitivity settings defined in the configuration
 * to the kinematics calculator.
 */
void applyHallSensitivityFromConfig() {
    if (g_hallKinematics == nullptr) {
        return;
    }

    // Apply sensitivity settings from defaults_hall.h
    g_hallKinematics->setSensitivity(0, DEF_SENS_TX_POS); // TX positive
    g_hallKinematics->setSensitivity(1, DEF_SENS_TY_POS); // TY positive
    g_hallKinematics->setSensitivity(2, DEF_SENS_TZ_POS); // TZ positive
    g_hallKinematics->setSensitivity(3, DEF_SENS_RX_POS); // RX positive
    g_hallKinematics->setSensitivity(4, DEF_SENS_RY_POS); // RY positive
    g_hallKinematics->setSensitivity(5, DEF_SENS_RZ_POS); // RZ positive
}
