#pragma once

class ICalibratorState;
class SensorCollection;

class Calibrator {
private:
    SensorCollection *m_SensorCollection = nullptr; // Pointer to the sensor collection
    ICalibratorState *m_currentState = nullptr;     // Pointer to the current calibrator state
public:
    Calibrator() = delete;
    Calibrator(SensorCollection *sensorCollection);
    virtual ~Calibrator() = default;

    const bool start(ICalibratorState *state);
    void finish();

    inline SensorCollection *getSensorCollection() const { return m_SensorCollection; }
};