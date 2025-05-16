#pragma once
#include <stdint.h>

class CollectionIdentifier;     // Forward declaration of CommandBase class
class SensorCalibrationManager; // Forward declaration of SensorCalibrationManager class
class Axis;
class AxisDirectionConfig; // Forward declaration of AxisDirectionConfig class
class DebugOutputAxes;
class DebugOutputSensors;
class DebugOutputLoopFrequency;

#define MAX_COMMANDS 11

class AVRCommandHandler {
private:
    CollectionIdentifier *m_CollectionIdentifier = nullptr;                                  // Pointer to the collection identifier
    CollectionIdentifier *getCollectionIdentifier() const { return m_CollectionIdentifier; } // Get the collection identifier

    SensorCalibrationManager *m_SensorCalibrationManager = nullptr; // Pointer to the sensor calibration manager
    Axis *m_Axis = nullptr;                                         // Pointer to the axis
    AxisDirectionConfig *m_DirectionConfig = nullptr;

    DebugOutputAxes *m_AxisObserver = nullptr;                   // Pointer to the axis observer instance
    DebugOutputSensors *m_SensorObserver = nullptr;              // Pointer to the sensor observer instance
    DebugOutputLoopFrequency *m_LoopFrequencyObserver = nullptr; // Pointer to the loop frequency observer instance

    void executeIdle(const char *param1, const char *param2, const uint8_t paramCount);
    void executeMinMax(const char *param1, const char *param2, const uint8_t paramCount);
    void executeDebug(const char *param1, const char *param2, const uint8_t paramCount);
    void executeSens(const char *param1, const char *param2, const uint8_t paramCount);
    void executeGate(const char *param1, const char *param2, const uint8_t paramCount);
    void executeModFunc(const char *param1, const char *param2, const uint8_t paramCount);
    void executeInvert(const char *param1, const char *param2, const uint8_t paramCount);
    void executeSwitchXY(const char *param1, const char *param2, const uint8_t paramCount);
    void executeExlc(const char *param1, const char *param2, const uint8_t paramCount);

    float executeAxis(const char *param1, const char *param2, const uint8_t paramCount);

    const bool convertWordNumber(const char *str, long *n) const;
    const bool convertWordFloat(const char *str, float *value) const;

    void DebugParamOff();
    void DebugParamSensorInformationRaw();
    void DebugParamSensorInformationCentered();
    void DebugParamSensorInformationFiltered();
    void DebugParamAxisInformation();
    void DebugParamSensorAxisInformation();
    void DebugParamSensorAxisKeysInformation();
    void DebugParamLoopFrequency();

    void DetachCurrentObservers();

public:
    AVRCommandHandler() = default;
    ~AVRCommandHandler() {
        DetachCurrentObservers(); // Detach the observers when the object is destroyed
    };

    CollectionIdentifier *setCollectionIdentifier(CollectionIdentifier *collectionIdentifier) { return m_CollectionIdentifier; } // Set the collection identifier

    void parseSerialMonitorInput();
    void handleInput(char input[], const uint8_t inputsize, const int8_t bytesRead);
};
