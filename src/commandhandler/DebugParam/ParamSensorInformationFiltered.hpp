#pragma once
#include "ParamSensorInformation.hpp"

class DebugParamSensorInformationFiltered : public DebugParamSensorInformation {
    // This class is responsible for handling the debug state related to sensor information.
    // It inherits from IDebugParam and implements the apply and report methods.
    // The class uses an instance of ODebugOutputSensors to output sensor values for debugging purposes.
public:
    DebugParamSensorInformationFiltered() = delete; // Delete the default constructor
    DebugParamSensorInformationFiltered(DebugCommand *context) : DebugParamSensorInformation(context) {}
    void apply() override;
    void report() override;
};
