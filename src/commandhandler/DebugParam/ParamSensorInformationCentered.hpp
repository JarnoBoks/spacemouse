#pragma once
#include "ParamSensorInformation.hpp"

class DebugParamSensorInformationCentered : public DebugParamSensorInformation {
    // This class is responsible for handling the debug state related to sensor information.
    // It inherits from IDebugParam and implements the apply and report methods.
    // The class uses an instance of ODebugOutputSensors to output sensor values for debugging purposes.
public:
    DebugParamSensorInformationCentered() = delete; // Delete the default constructor
    DebugParamSensorInformationCentered(DebugCommand *context) : DebugParamSensorInformation(context) {}
    void apply() override;
    void report() override;
};
