/**
 * @file HallKinematicsTest.cpp
 * @brief Simple test for the HallSensorKinematics algorithm
 *
 * This file provides a basic test to verify the algorithm compiles
 * and produces reasonable output values.
 *
 * @author GitHub Copilot
 */

#include <Arduino.h>
#include "HallSensorKinematics.hpp"
#include "HallKinematicsWrapper.cpp"

// Test sensor readings (simulated values)
struct TestCase {
    const char *name;
    int sensorReadings[8];
    int expectedMotion[6]; // Expected approximate output [TX,TY,TZ,RX,RY,RZ]
};

// Test cases for validation
TestCase testCases[] = {
    {
        "Idle Position",
        {3643, 3653, 3555, 3647, 3540, 3497, 3658, 3657}, // All at electrical zeroes
        {0, 0, 0, 0, 0, 0}                                // Expect no motion
    },
    {
        "Translation +X",
        {3643, 3653, 3555, 3647, 3600, 3557, 3658, 3657}, // HES5,HES6 increased
        {200, 0, 0, 0, 0, 0}                              // Expect positive X translation
    },
    {
        "Translation +Y",
        {3700, 3710, 3555, 3647, 3540, 3497, 3658, 3657}, // HES1,HES2 increased
        {0, 200, 0, 0, 0, 0}                              // Expect positive Y translation
    },
    {
        "Translation +Z",
        {3700, 3710, 3600, 3700, 3600, 3557, 3715, 3714}, // All sensors increased
        {0, 0, 200, 0, 0, 0}                              // Expect positive Z translation
    },
    {
        "Rotation Roll (+RX)",
        {3700, 3710, 3500, 3597, 3540, 3497, 3658, 3657}, // 12 o'clock up, 6 o'clock down
        {0, 0, 0, 150, 0, 0}                              // Expect positive roll
    }};

const int numTestCases = sizeof(testCases) / sizeof(TestCase);

void setup() {
    Serial.begin(115200);
    while (!Serial)
        delay(10);

    Serial.println("Hall Sensor Kinematics Algorithm Test");
    Serial.println("====================================");

    // Initialize the kinematics algorithm
    HallSensorKinematics kinematics;

    // Run test cases
    for (int i = 0; i < numTestCases; i++) {
        TestCase &test = testCases[i];
        int16_t motionVectors[6];

        // Calculate motion
        kinematics.calculateMotion(test.sensorReadings, motionVectors);

        // Print results
        Serial.print("Test: ");
        Serial.println(test.name);
        Serial.print("Input:  ");
        for (int j = 0; j < 8; j++) {
            Serial.print(test.sensorReadings[j]);
            Serial.print(" ");
        }
        Serial.println();

        Serial.print("Output: TX=");
        Serial.print(motionVectors[0]);
        Serial.print(" TY=");
        Serial.print(motionVectors[1]);
        Serial.print(" TZ=");
        Serial.print(motionVectors[2]);
        Serial.print(" RX=");
        Serial.print(motionVectors[3]);
        Serial.print(" RY=");
        Serial.print(motionVectors[4]);
        Serial.print(" RZ=");
        Serial.println(motionVectors[5]);

        Serial.print("Expect: TX=");
        Serial.print(test.expectedMotion[0]);
        Serial.print(" TY=");
        Serial.print(test.expectedMotion[1]);
        Serial.print(" TZ=");
        Serial.print(test.expectedMotion[2]);
        Serial.print(" RX=");
        Serial.print(test.expectedMotion[3]);
        Serial.print(" RY=");
        Serial.print(test.expectedMotion[4]);
        Serial.print(" RZ=");
        Serial.println(test.expectedMotion[5]);

        Serial.println("---");
    }

    // Test C-style wrapper
    Serial.println("Testing C-style wrapper:");
    setHallCalibrationFromConfig();
    applyHallSensitivityFromConfig();

    int testSensors[8] = {3643, 3653, 3555, 3647, 3540, 3497, 3658, 3657};
    int16_t testOutput[6];

    calculateHallKinematics(testSensors, testOutput);

    Serial.print("Wrapper test output: ");
    for (int i = 0; i < 6; i++) {
        Serial.print(testOutput[i]);
        Serial.print(" ");
    }
    Serial.println();

    Serial.println("Test completed successfully!");
}

void loop() {
    // Nothing to do in loop
    delay(1000);
}

/**
 * Expected behavior:
 * - Idle position should produce near-zero outputs
 * - +X translation should produce positive TX value
 * - +Y translation should produce positive TY value
 * - +Z translation should produce positive TZ value
 * - Roll rotation should produce positive RX value
 * - All other axes should remain near zero for single-axis tests
 *
 * If outputs are inverted or cross-coupled, check:
 * 1. Sensor pair mapping in calculateMotion()
 * 2. Sensitivity matrix in initializeSensitivityMatrix()
 * 3. Calibration values (electrical zeroes, min/max)
 */
