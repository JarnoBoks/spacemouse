/**
 * @file HallKinematicsExample.cpp
 * @brief Example usage of the HallSensorKinematics algorithm
 *
 * This example demonstrates how to use the advanced Hall sensor kinematics
 * algorithm in various scenarios and how to integrate it with existing code.
 *
 * @author GitHub Copilot
 */

#include <Arduino.h>
#include "HallSensorKinematics.hpp"
#include "HallKinematicsWrapper.cpp"

// Example sensor pin definitions (ESP32)
const int SENSOR_PINS[8] = {A0, A1, A2, A3, A4, A5, A6, A7};

/**
 * @brief Example 1: Basic usage with the C++ class
 */
void example_basic_cpp_usage() {
    Serial.println("=== Example 1: Basic C++ Class Usage ===");

    // Create kinematics processor
    HallSensorKinematics kinematics;

    // Simulate sensor readings (normally from analogRead)
    int sensorReadings[8] = {3650, 3645, 3560, 3650, 3545, 3500, 3660, 3655};
    int16_t motionVectors[6];

    // Calculate motion
    kinematics.calculateMotion(sensorReadings, motionVectors);

    // Print results
    Serial.println("Motion Vectors:");
    Serial.print("TX: ");
    Serial.print(motionVectors[0]);
    Serial.print(", TY: ");
    Serial.print(motionVectors[1]);
    Serial.print(", TZ: ");
    Serial.print(motionVectors[2]);
    Serial.print(", RX: ");
    Serial.print(motionVectors[3]);
    Serial.print(", RY: ");
    Serial.print(motionVectors[4]);
    Serial.print(", RZ: ");
    Serial.println(motionVectors[5]);
}

/**
 * @brief Example 2: Using the C-style wrapper
 */
void example_c_wrapper_usage() {
    Serial.println("=== Example 2: C-Style Wrapper Usage ===");

    // Initialize kinematics
    HallSensorKinematics *kinematics = hallKinematics_init();

    // Set up calibration values
    int zeroes[8] = {3643, 3653, 3555, 3647, 3540, 3497, 3658, 3657};
    int mins[8] = {-512, -512, -512, -512, -512, -512, -512, -512};
    int maxs[8] = {512, 512, 512, 512, 512, 512, 512, 512};

    hallKinematics_setCalibration(kinematics, zeroes, mins, maxs);

    // Adjust sensitivity for more responsive pitch
    hallKinematics_setSensitivity(kinematics, 4, 1.5f); // RY axis

    // Read sensors and calculate motion
    int sensorReadings[8];
    for (int i = 0; i < 8; i++) {
        sensorReadings[i] = analogRead(SENSOR_PINS[i]);
    }

    int16_t motionVectors[6];
    hallKinematics_calculate(kinematics, sensorReadings, motionVectors);

    // Print results
    Serial.println("Motion with adjusted sensitivity:");
    Serial.print("TX: ");
    Serial.print(motionVectors[0]);
    Serial.print(", TY: ");
    Serial.print(motionVectors[1]);
    Serial.print(", TZ: ");
    Serial.print(motionVectors[2]);
    Serial.print(", RX: ");
    Serial.print(motionVectors[3]);
    Serial.print(", RY: ");
    Serial.print(motionVectors[4]);
    Serial.print(", RZ: ");
    Serial.println(motionVectors[5]);
}

/**
 * @brief Example 3: Integration with existing calculateKinematic style
 */
void example_legacy_integration() {
    Serial.println("=== Example 3: Legacy Integration ===");

    // Initialize calibration from config
    setHallCalibrationFromConfig();
    applyHallSensitivityFromConfig();

    // Read sensors (this would normally be done by existing sensor reading code)
    int centered[8];
    for (int i = 0; i < 8; i++) {
        int raw = analogRead(SENSOR_PINS[i]);
        // Center around electrical zero (simplified)
        centered[i] = raw - 3600; // Approximate electrical zero
    }

    // Calculate kinematics using drop-in replacement function
    int16_t velocity[6];
    calculateHallKinematics(centered, velocity);

    // Print results
    Serial.println("Legacy-style integration:");
    Serial.print("TX: ");
    Serial.print(velocity[0]);
    Serial.print(", TY: ");
    Serial.print(velocity[1]);
    Serial.print(", TZ: ");
    Serial.print(velocity[2]);
    Serial.print(", RX: ");
    Serial.print(velocity[3]);
    Serial.print(", RY: ");
    Serial.print(velocity[4]);
    Serial.print(", RZ: ");
    Serial.println(velocity[5]);
}

/**
 * @brief Example 4: Calibration procedure
 */
void example_calibration_procedure() {
    Serial.println("=== Example 4: Calibration Procedure ===");

    HallSensorKinematics kinematics;

    // Step 1: Measure electrical zeroes
    Serial.println("Step 1: Place spacemouse in idle position and measure zeroes...");
    delay(2000);

    long sums[8] = {0};
    const int numSamples = 500;

    for (int sample = 0; sample < numSamples; sample++) {
        for (int i = 0; i < 8; i++) {
            sums[i] += analogRead(SENSOR_PINS[i]);
        }
        delay(1);
    }

    int zeroes[8];
    for (int i = 0; i < 8; i++) {
        zeroes[i] = sums[i] / numSamples;
        Serial.print("Sensor ");
        Serial.print(i);
        Serial.print(" zero: ");
        Serial.println(zeroes[i]);
    }

    // Step 2: Measure ranges (would require user interaction)
    Serial.println("Step 2: Move spacemouse through full range to measure min/max...");
    Serial.println("(In real calibration, capture min/max values during movement)");

    // For demo, use default ranges
    int mins[8] = {-512, -512, -512, -512, -512, -512, -512, -512};
    int maxs[8] = {512, 512, 512, 512, 512, 512, 512, 512};

    // Apply calibration
    kinematics.setCalibration(zeroes, mins, maxs);

    Serial.println("Calibration complete!");
}

/**
 * @brief Example 5: Real-time monitoring with cross-coupling analysis
 */
void example_realtime_monitoring() {
    Serial.println("=== Example 5: Real-time Monitoring ===");

    HallSensorKinematics kinematics;

    // Setup calibration
    setHallCalibrationFromConfig();

    Serial.println("Monitoring motion for 10 seconds...");
    Serial.println("Time(ms)\tTX\tTY\tTZ\tRX\tRY\tRZ");

    unsigned long startTime = millis();
    while (millis() - startTime < 10000) { // 10 seconds
        // Read sensors
        int sensorReadings[8];
        for (int i = 0; i < 8; i++) {
            sensorReadings[i] = analogRead(SENSOR_PINS[i]);
        }

        // Calculate motion
        int16_t motionVectors[6];
        kinematics.calculateMotion(sensorReadings, motionVectors);

        // Print data (every 100ms)
        static unsigned long lastPrint = 0;
        if (millis() - lastPrint >= 100) {
            Serial.print(millis());
            for (int i = 0; i < 6; i++) {
                Serial.print("\t");
                Serial.print(motionVectors[i]);
            }
            Serial.println();
            lastPrint = millis();
        }

        delay(10); // 100Hz update rate
    }
}

/**
 * @brief Main setup function
 */
void setup() {
    Serial.begin(115200);
    while (!Serial)
        delay(10);

    Serial.println("Hall Sensor Kinematics Examples");
    Serial.println("================================");

    // Run examples
    example_basic_cpp_usage();
    delay(1000);

    example_c_wrapper_usage();
    delay(1000);

    example_legacy_integration();
    delay(1000);

    example_calibration_procedure();
    delay(1000);

    example_realtime_monitoring();

    Serial.println("All examples completed!");
}

/**
 * @brief Main loop (not used in this example)
 */
void loop() {
    // Examples run in setup(), nothing to do here
    delay(1000);
}

/**
 * INTEGRATION NOTES:
 * ==================
 *
 * To integrate this algorithm into the existing spacemouse codebase:
 *
 * 1. Replace calculateKinematic() calls with calculateHallKinematics()
 *
 * 2. In setup(), call:
 *    setHallCalibrationFromConfig();
 *    applyHallSensitivityFromConfig();
 *
 * 3. Modify SensorsCalculatorHall.cpp to use the new algorithm:
 *    - Include HallKinematicsWrapper.cpp
 *    - Replace the switch statement with a call to calculateHallKinematics()
 *
 * 4. Update calibration routines to work with the new setCalibration() method
 *
 * 5. Test and tune the COUPLING_COMPENSATION and other constants for your hardware
 *
 * The algorithm maintains the same input/output interface as the existing code,
 * making integration straightforward while providing improved cross-axis decoupling.
 */
