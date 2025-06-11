#pragma once
#include "ICalibratorState.hpp"             // Include the ICalibratorState interface header file
#include <sensor/calibrator/Calibrator.hpp> // Include the Calibrator class header file

class Calibrator;
class IObserver;

class CalibratorStateBase : public ICalibratorState {
    // Base class for all calibrator states, providing common functionality and interface
    // for managing the calibration process of the sensors in the SpaceMouse.
protected:
    IObserver *sensorObserver = nullptr;      // Pointer to the (sensor) observer associated with this state
    unsigned long m_startCalibrationTime = 0; // Calibration start time from millis()
public:
    CalibratorStateBase() = default; // Default constructor
    virtual ~CalibratorStateBase();
};