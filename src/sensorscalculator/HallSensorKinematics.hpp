/**
 * @file HallSensorKinematics.hpp
 * @brief Header file for advanced Hall sensor 6-DOF motion calculation
 *
 * @author GitHub Copilot
 */

#pragma once

#include <Arduino.h>

// Physical constants - adjust these based on your hardware
#define SENSOR_RADIUS_MM 18.6f      // Distance from center to sensor pairs (mm)
#define MAGNET_HEIGHT_MM 5.0f       // Height of magnet plane above sensors (mm)
#define COUPLING_COMPENSATION 0.85f // Factor to reduce cross-coupling (0.0-1.0)
#define OUTPUT_SCALE 350.0f         // Output scaling factor
#define DEADZONE_THRESHOLD 5        // Values below this are set to zero

// Sensor pair positions (clockwise from 12 o'clock)
enum SensorPair {
    PAIR_12_OCLOCK = 0, // North  (Y+)
    PAIR_3_OCLOCK = 1,  // East   (X+)
    PAIR_6_OCLOCK = 2,  // South  (Y-)
    PAIR_9_OCLOCK = 3   // West   (X-)
};

/**
 * @brief Advanced Hall sensor processor for 6-DOF motion calculation
 */
class HallSensorKinematics {
private:
    // Calibration values - should be determined during calibration
    int electricZeroes[8];         // Idle position for each sensor
    int minVals[8];                // Minimum calibrated values
    int maxVals[8];                // Maximum calibrated values
    float sensitivityMatrix[6][4]; // Sensitivity matrix for each axis vs sensor pair

    // Internal state
    float lastRotation[3]; // Previous rotation values for filtering
    float rotationFilter;  // Low-pass filter coefficient for rotation

    void initializeSensitivityMatrix();
    float processSensorPair(int sensorA, int sensorB, int pairIndex);
    float calculatePairAsymmetry(int sensorA, int sensorB, int pairIndex);

public:
    HallSensorKinematics();
    void calculateMotion(int sensorReadings[8], int16_t motionVectors[6]);
    void setCalibration(int zeroes[8], int mins[8], int maxs[8]);
    void setSensitivity(int axis, float sensitivity);
};

// Function prototypes for C-style interface (compatible with existing codebase)
extern "C" {
HallSensorKinematics *hallKinematics_init();
void hallKinematics_calculate(HallSensorKinematics *instance, int sensorReadings[8], int16_t motionVectors[6]);
void hallKinematics_setCalibration(HallSensorKinematics *instance, int zeroes[8], int mins[8], int maxs[8]);
void hallKinematics_setSensitivity(HallSensorKinematics *instance, int axis, float sensitivity);
void hallKinematics_destroy(HallSensorKinematics *instance);
}

// Legacy integration functions
void calculateHallKinematics(int *centered, int16_t *velocity);
void setHallCalibrationFromConfig();
void applyHallSensitivityFromConfig();
