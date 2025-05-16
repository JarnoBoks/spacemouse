#pragma once

#include "IPrinterVisitor.h"
#include "sensor/sensors/Sensor.hpp"
#include "sensor/config/SensorConfig.h"
#include "kinematics/kinematicsconfig.h"
#include "common/TextHelper.h"

class ExclusiveModePrinter : public IPrinterVisitor {
public:
    ExclusiveModePrinter() {
        // Constructor to initialize the ExclusiveModePrinter object
        Serial.println(F("Exclusive Mode: "));
    }

    inline void visit(Sensor &sensor) override {}
    inline void visit(SensorConfig &config) override {}
    inline void visit(Axis &axis) override {};
    inline void visit(AxisConfig &config) override {};
    void visit(KinematicsConfig &config) override {
        bool exclusiveMode = config.exclusiveMode; // Get the minimum value from the sensor configuration
        Serial.print(exclusiveMode);
        TextHelper::printBooleanDescription(exclusiveMode);
        Serial.println();
    }
};
