/**
 * @file HallSensorKinematics.cpp
 * @brief Advanced Hall sensor algorithm for 6-DOF motion calculation
 *
 * This algorithm translates readings from 4 Hall sensor pairs to 6 motion vectors
 * (3 translation + 3 rotation) with improved cross-axis coupling compensation.
 *
 * Sensor Layout (top view):
 * - Pair A (12 o'clock): HES1, HES2
 * - Pair B (3 o'clock):  HES5, HES6
 * - Pair C (6 o'clock):  HES3, HES4
 * - Pair D (9 o'clock):  HES7, HES8
 *
 * Each pair has magnets positioned above them in the same plane.
 * Output range: -350 to +350 for each motion vector
 *
 * @author GitHub Copilot
 */

#include "HallSensorKinematics.hpp"
#include <math.h>

// Include defaults if available
#ifdef ELECTRICZEROES
// Use defaults from defaults_hall.h if available
#else
// Define fallback defaults if config not available
#define ELECTRICZEROES {3643, 3653, 3555, 3647, 3540, 3497, 3658, 3657}
#define MINVALS {-512, -512, -512, -512, -512, -512, -512, -512}
#define MAXVALS {512, 512, 512, 512, 512, 512, 512, 512}
#endif

/**
 * @brief Constructor - initializes with default calibration values
 */
HallSensorKinematics::HallSensorKinematics() {
    // Initialize with defaults from defaults_hall.h
    int defaultZeroes[8] = ELECTRICZEROES;
    int defaultMins[8] = MINVALS;
    int defaultMaxs[8] = MAXVALS;

    for (int i = 0; i < 8; i++) {
        electricZeroes[i] = defaultZeroes[i];
        minVals[i] = defaultMins[i];
        maxVals[i] = defaultMaxs[i];
    }

    initializeSensitivityMatrix();

    // Initialize rotation filter
    rotationFilter = 0.7f; // Adjust for smoother rotation response
    for (int i = 0; i < 3; i++) {
        lastRotation[i] = 0.0f;
    }
}

/**
 * @brief Initialize the sensitivity matrix based on geometric relationships
 */
void HallSensorKinematics::initializeSensitivityMatrix() {
    // Reset matrix
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            sensitivityMatrix[i][j] = 0.0f;
        }
    }

    // Translation X (East-West movement)
    // Affected primarily by 3 o'clock and 9 o'clock pairs
    sensitivityMatrix[0][PAIR_3_OCLOCK] = 1.0f;  // Positive X
    sensitivityMatrix[0][PAIR_9_OCLOCK] = -1.0f; // Negative X

    // Translation Y (North-South movement)
    // Affected primarily by 12 o'clock and 6 o'clock pairs
    sensitivityMatrix[1][PAIR_12_OCLOCK] = 1.0f; // Positive Y
    sensitivityMatrix[1][PAIR_6_OCLOCK] = -1.0f; // Negative Y

    // Translation Z (Up-Down movement)
    // All sensors affected equally (same sign)
    sensitivityMatrix[2][PAIR_12_OCLOCK] = 1.0f;
    sensitivityMatrix[2][PAIR_3_OCLOCK] = 1.0f;
    sensitivityMatrix[2][PAIR_6_OCLOCK] = 1.0f;
    sensitivityMatrix[2][PAIR_9_OCLOCK] = 1.0f;

    // Rotation X (Roll - tilt around X axis)
    // 12 and 6 o'clock pairs have opposite signs
    sensitivityMatrix[3][PAIR_12_OCLOCK] = 1.0f;
    sensitivityMatrix[3][PAIR_6_OCLOCK] = -1.0f;

    // Rotation Y (Pitch - tilt around Y axis)
    // 3 and 9 o'clock pairs have opposite signs
    sensitivityMatrix[4][PAIR_3_OCLOCK] = 1.0f;
    sensitivityMatrix[4][PAIR_9_OCLOCK] = -1.0f;

    // Rotation Z (Yaw - rotation around Z axis)
    // Diagonal pattern for yaw detection
    sensitivityMatrix[5][PAIR_12_OCLOCK] = 1.0f;
    sensitivityMatrix[5][PAIR_3_OCLOCK] = -1.0f;
    sensitivityMatrix[5][PAIR_6_OCLOCK] = 1.0f;
    sensitivityMatrix[5][PAIR_9_OCLOCK] = -1.0f;
}

/**
 * @brief Read and normalize sensor values from one pair
 * @param sensorA First sensor in pair (raw ADC value)
 * @param sensorB Second sensor in pair (raw ADC value)
 * @param pairIndex Index of the sensor pair (0-3)
 * @return Normalized average value of the pair
 */
float HallSensorKinematics::processSensorPair(int sensorA, int sensorB, int pairIndex) {
    // Calculate indices for the sensor pair
    int idxA = pairIndex * 2;
    int idxB = pairIndex * 2 + 1;

    // Center the values around electrical zero
    float centeredA = sensorA - electricZeroes[idxA];
    float centeredB = sensorB - electricZeroes[idxB];

    // Normalize to -1.0 to +1.0 range
    float normalizedA = centeredA / (maxVals[idxA] - minVals[idxA]) * 2.0f;
    float normalizedB = centeredB / (maxVals[idxB] - minVals[idxB]) * 2.0f;

    // Return average of the pair
    return (normalizedA + normalizedB) * 0.5f;
}

/**
 * @brief Calculate asymmetry in sensor pair for rotation detection
 * @param sensorA First sensor in pair (raw ADC value)
 * @param sensorB Second sensor in pair (raw ADC value)
 * @param pairIndex Index of the sensor pair (0-3)
 * @return Asymmetry value (difference between sensors)
 */
float HallSensorKinematics::calculatePairAsymmetry(int sensorA, int sensorB, int pairIndex) {
    int idxA = pairIndex * 2;
    int idxB = pairIndex * 2 + 1;

    float centeredA = sensorA - electricZeroes[idxA];
    float centeredB = sensorB - electricZeroes[idxB];

    float normalizedA = centeredA / (maxVals[idxA] - minVals[idxA]) * 2.0f;
    float normalizedB = centeredB / (maxVals[idxB] - minVals[idxB]) * 2.0f;

    return normalizedA - normalizedB;
}

/**
 * @brief Main algorithm to calculate 6-DOF motion from 8 Hall sensor readings
 * @param sensorReadings Array of 8 raw sensor readings [HES1, HES2, HES3, HES4, HES5, HES6, HES7, HES8]
 * @param motionVectors Output array of 6 motion vectors [TX, TY, TZ, RX, RY, RZ]
 */
void HallSensorKinematics::calculateMotion(int sensorReadings[8], int16_t motionVectors[6]) {
    // Process sensor pairs to get average readings
    float pairAverages[4];
    float pairAsymmetries[4];

    // ESP32 sensor mapping: [HES1, HES2, HES3, HES4, HES5, HES6, HES7, HES8]
    // Pair mapping: 12-o'clock=HES1,HES2; 3-o'clock=HES5,HES6; 6-o'clock=HES3,HES4; 9-o'clock=HES7,HES8
    pairAverages[PAIR_12_OCLOCK] = processSensorPair(sensorReadings[0], sensorReadings[1], PAIR_12_OCLOCK); // HES1, HES2
    pairAverages[PAIR_3_OCLOCK] = processSensorPair(sensorReadings[4], sensorReadings[5], PAIR_3_OCLOCK);   // HES5, HES6
    pairAverages[PAIR_6_OCLOCK] = processSensorPair(sensorReadings[2], sensorReadings[3], PAIR_6_OCLOCK);   // HES3, HES4
    pairAverages[PAIR_9_OCLOCK] = processSensorPair(sensorReadings[6], sensorReadings[7], PAIR_9_OCLOCK);   // HES7, HES8

    // Calculate asymmetries for improved rotation detection
    pairAsymmetries[PAIR_12_OCLOCK] = calculatePairAsymmetry(sensorReadings[0], sensorReadings[1], PAIR_12_OCLOCK);
    pairAsymmetries[PAIR_3_OCLOCK] = calculatePairAsymmetry(sensorReadings[4], sensorReadings[5], PAIR_3_OCLOCK);
    pairAsymmetries[PAIR_6_OCLOCK] = calculatePairAsymmetry(sensorReadings[2], sensorReadings[3], PAIR_6_OCLOCK);
    pairAsymmetries[PAIR_9_OCLOCK] = calculatePairAsymmetry(sensorReadings[6], sensorReadings[7], PAIR_9_OCLOCK);

    // Phase 1: Calculate raw motion vectors using sensitivity matrix
    float rawMotion[6];
    for (int axis = 0; axis < 6; axis++) {
        rawMotion[axis] = 0.0f;
        for (int pair = 0; pair < 4; pair++) {
            rawMotion[axis] += sensitivityMatrix[axis][pair] * pairAverages[pair];
        }
    }

    // Phase 2: Enhance rotation detection using asymmetries
    // Rotation is better detected from asymmetries within pairs
    float enhancedRotation[3];

    // Roll (RX): asymmetry between 12 and 6 o'clock pairs
    enhancedRotation[0] = (pairAsymmetries[PAIR_12_OCLOCK] - pairAsymmetries[PAIR_6_OCLOCK]) * 0.5f;

    // Pitch (RY): asymmetry between 3 and 9 o'clock pairs
    enhancedRotation[1] = (pairAsymmetries[PAIR_3_OCLOCK] - pairAsymmetries[PAIR_9_OCLOCK]) * 0.5f;

    // Yaw (RZ): circular asymmetry pattern
    enhancedRotation[2] = (pairAsymmetries[PAIR_12_OCLOCK] - pairAsymmetries[PAIR_3_OCLOCK] +
                           pairAsymmetries[PAIR_6_OCLOCK] - pairAsymmetries[PAIR_9_OCLOCK]) *
                          0.25f;

    // Phase 3: Cross-coupling compensation
    // Compensate translation Z for rotation effects
    float rotationMagnitude = sqrt(enhancedRotation[0] * enhancedRotation[0] +
                                   enhancedRotation[1] * enhancedRotation[1] +
                                   enhancedRotation[2] * enhancedRotation[2]);

    if (rotationMagnitude > 0.1f) {
        // Reduce TZ when significant rotation is detected
        rawMotion[2] *= (1.0f - COUPLING_COMPENSATION * rotationMagnitude);
    }

    // Compensate translation X,Y for yaw rotation
    if (abs(enhancedRotation[2]) > 0.05f) {
        rawMotion[0] *= (1.0f - 0.3f * abs(enhancedRotation[2])); // Reduce TX during yaw
        rawMotion[1] *= (1.0f - 0.3f * abs(enhancedRotation[2])); // Reduce TY during yaw
    }

    // Phase 4: Apply filtering to rotations for smoother response
    for (int i = 0; i < 3; i++) {
        enhancedRotation[i] = rotationFilter * lastRotation[i] + (1.0f - rotationFilter) * enhancedRotation[i];
        lastRotation[i] = enhancedRotation[i];
    }

    // Phase 5: Scale to output range and apply deadzone
    for (int i = 0; i < 3; i++) {
        // Translations
        motionVectors[i] = (int16_t)(rawMotion[i] * OUTPUT_SCALE);
        // Rotations
        motionVectors[i + 3] = (int16_t)(enhancedRotation[i] * OUTPUT_SCALE);
    }

    // Apply deadzone
    for (int i = 0; i < 6; i++) {
        if (abs(motionVectors[i]) < DEADZONE_THRESHOLD) {
            motionVectors[i] = 0;
        }
        // Constrain to output range
        motionVectors[i] = constrain(motionVectors[i], -350, 350);
    }
}

/**
 * @brief Set calibration values for sensors
 * @param zeroes Array of 8 electrical zero positions
 * @param mins Array of 8 minimum values
 * @param maxs Array of 8 maximum values
 */
void HallSensorKinematics::setCalibration(int zeroes[8], int mins[8], int maxs[8]) {
    for (int i = 0; i < 8; i++) {
        electricZeroes[i] = zeroes[i];
        minVals[i] = mins[i];
        maxVals[i] = maxs[i];
    }
}

/**
 * @brief Adjust sensitivity for specific axis
 * @param axis Motion axis (0=TX, 1=TY, 2=TZ, 3=RX, 4=RY, 5=RZ)
 * @param sensitivity Sensitivity multiplier (1.0 = default)
 */
void HallSensorKinematics::setSensitivity(int axis, float sensitivity) {
    if (axis >= 0 && axis < 6) {
        for (int pair = 0; pair < 4; pair++) {
            sensitivityMatrix[axis][pair] *= sensitivity;
        }
    }
}
