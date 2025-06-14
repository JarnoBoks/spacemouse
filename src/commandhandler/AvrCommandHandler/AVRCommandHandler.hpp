#pragma once
#include <stdint.h>

class CollectionCarrier;        // Forward declaration of CommandBase class
class SensorCalibrationManager; // Forward declaration of SensorCalibrationManager class
class KnobAxis;
class KnobAxisDirectionConfig; // Forward declaration of AxisDirectionConfig class
class DebugOutputAxes;
class DebugOutputSensors;
class DebugOutputLoopFrequency;

#define MAX_COMMANDS 11
#define NUM_AX_DIRCFG 2 // Number of axis direction configurations in the software

class AVRCommandHandler {
private:
    CollectionCarrier *m_CollectionCarrier = nullptr;                               // Pointer to the collection carrier
    CollectionCarrier *getCollectionCarrier() const { return m_CollectionCarrier; } // Get the collection carrier

    SensorCalibrationManager *m_SensorCalibrationManager = nullptr;                         // Pointer to the sensor calibration manager
    KnobAxis *m_knobAxis = nullptr;                                                         // Pointer to the knob Axis
    KnobAxisDirectionConfig *m_knobAxisDirectionConfig[NUM_AX_DIRCFG] = {nullptr, nullptr}; // Pointer to the knob Axis direction configuration

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
    const bool convertWordBool(const char *str, bool *n) const;

    void DebugParamOff();
    void DebugParamSensorInformationRaw();
    void DebugParamSensorInformationCentered();
    void DebugParamSensorInformationFiltered();
    void DebugParamAxisInformation();
    void DebugParamSensorAxisInformation();
    void DebugParamSensorAxisKeysInformation();
    void DebugParamSensorAxisKeysInformationExclusive();
    void DebugParamLoopFrequency();

    void DetachCurrentObservers();

public:
    AVRCommandHandler() = default;
    ~AVRCommandHandler() {
        DetachCurrentObservers(); // Detach the observers when the object is destroyed
    };

    void setCollectionIdentifier(CollectionCarrier *collectionCarrier) { m_CollectionCarrier = collectionCarrier; }

    void parseSerialMonitorInput();
    void handleInput(char input[], const uint8_t inputsize, const int8_t bytesRead);
};
