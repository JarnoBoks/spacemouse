#pragma once
#include "ICalibratorState.hpp"             // Include the ICalibratorState interface header file
#include <sensor/calibrator/Calibrator.hpp> // Include the Calibrator class header file

class Calibrator;
class IObserver;

class CalibratorStateBase : public ICalibratorState {
    // Base class for all calibrator states, providing common functionality and interface
    // for managing the calibration process of the sensors in the SpaceMouse.
protected:
    Calibrator *context = nullptr;            // Pointer to the context (Calibrator) that this state belongs to
    IObserver *sensorObserver = nullptr;      // Pointer to the (sensor) observer associated with this state
    unsigned long m_startCalibrationTime = 0; // Calibration start time from millis()
public:
    CalibratorStateBase() = default; // Default constructor
    virtual ~CalibratorStateBase();

#if 0
    // REVIEW - Is there a need of an interface file (to make apply override something)?
    virtual void start() = 0;
    virtual void update() = 0;
    virtual void finish() = 0;
#endif
};