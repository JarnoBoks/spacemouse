#ifndef DP_PARAMSENSORINFORMATION_H
#define DP_PARAMSENSORINFORMATION_H
#include "IDebugParam.h"
#include "serialoutput/output_sensorvalues.h"

class IDebugParamSensorInformation : public IDebugParam {
    // This class is responsible for handling the debug state related to sensor information.
    // It inherits from IDebugParam and implements the apply and report methods.
    // The class uses an instance of Output_SensorValues to output sensor values for debugging purposes.
protected:
    Output_SensorValues *SensorObserver = nullptr; // Pointer to the raw sensor observer instance
public:
    IDebugParamSensorInformation();  // Constructor
    ~IDebugParamSensorInformation(); // Destructor to clean up the observer instance
    virtual void apply() override = 0;
    virtual void report() override = 0;
};

class DebugParamSensorInformationRaw : public IDebugParamSensorInformation {
    // This class is responsible for handling the debug state related to sensor information.
    // It inherits from IDebugParam and implements the apply and report methods.
    // The class uses an instance of Output_SensorValues to output sensor values for debugging purposes.
public:
    void apply() override;
    void report() override;
};

class DebugParamSensorInformationCentered : public IDebugParamSensorInformation {
    // This class is responsible for handling the debug state related to sensor information.
    // It inherits from IDebugParam and implements the apply and report methods.
    // The class uses an instance of Output_SensorValues to output sensor values for debugging purposes.
public:
    void apply() override;
    void report() override;
};

class DebugParamSensorInformationFiltered : public IDebugParamSensorInformation {
    // This class is responsible for handling the debug state related to sensor information.
    // It inherits from IDebugParam and implements the apply and report methods.
    // The class uses an instance of Output_SensorValues to output sensor values for debugging purposes.
public:
    void apply() override;
    void report() override;
};

#endif // DP_PARAMSENSORINFORMATION_H