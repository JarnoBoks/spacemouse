#pragma once
#include "ParamSensorInformation.hpp"

class DebugParamSensorInformationRaw : public DebugParamSensorInformation {
    // This class is responsible for handling the debug state related to sensor information.
    // It inherits from IDebugParam and implements the apply and report methods.
    // The class uses an instance of ODebugOutputSensors to output sensor values for debugging purposes.
public:
    DebugParamSensorInformationRaw() = delete; // Delete the default constructor
    DebugParamSensorInformationRaw(DebugCommand *context) : DebugParamSensorInformation(context) {}
    ~DebugParamSensorInformationRaw();
    void apply() override;
    void report() override;
};
