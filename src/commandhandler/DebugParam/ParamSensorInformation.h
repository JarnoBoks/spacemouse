#pragma once
#include "IDebugParam.hpp"
#include "..\..\observers\DebugOutput\DebugOutputSensors.hpp" // Base class for sensor observers       //REVIEW - Can we use the Interface instead of the base class?

// REFACTOR - Split this file into multiple class files.
class IDebugParamSensorInformation : public IDebugParam {
    // This class is responsible for handling the debug state related to sensor information.
    // It inherits from IDebugParam and implements the apply and report methods.
    // The class uses an instance of ODebugOutputSensors to output sensor values for debugging purposes.
protected:
    DebugOutputSensors *SensorObserver = nullptr; // Pointer to the raw sensor observer instance       // REVIEW - Why is this necesary?
public:
    IDebugParamSensorInformation();          // Constructor
    virtual ~IDebugParamSensorInformation(); // Destructor to clean up the observer instance
    virtual void apply() override = 0;
    virtual void report() override = 0;
};

class DebugParamSensorInformationRaw : public IDebugParamSensorInformation {
    // This class is responsible for handling the debug state related to sensor information.
    // It inherits from IDebugParam and implements the apply and report methods.
    // The class uses an instance of ODebugOutputSensors to output sensor values for debugging purposes.
public:
    DebugParamSensorInformationRaw();
    ~DebugParamSensorInformationRaw();
    void apply() override;
    void report() override;
};

class DebugParamSensorInformationCentered : public IDebugParamSensorInformation {
    // This class is responsible for handling the debug state related to sensor information.
    // It inherits from IDebugParam and implements the apply and report methods.
    // The class uses an instance of ODebugOutputSensors to output sensor values for debugging purposes.
public:
    void apply() override;
    void report() override;
};

class DebugParamSensorInformationFiltered : public IDebugParamSensorInformation {
    // This class is responsible for handling the debug state related to sensor information.
    // It inherits from IDebugParam and implements the apply and report methods.
    // The class uses an instance of ODebugOutputSensors to output sensor values for debugging purposes.
public:
    void apply() override;
    void report() override;
};
