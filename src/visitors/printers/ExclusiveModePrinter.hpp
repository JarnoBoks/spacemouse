#pragma once

#include "IPrinterVisitor.h"
#include "sensor/sensors/Sensor.hpp"
#include "sensor/config/SensorConfig.hpp"
#include "kinematics/config/kinematicsconfig.hpp"
#include "common/TextHelper.h"

class ExclusiveModePrinter : public IPrinterVisitor {
public:
    ExclusiveModePrinter() {
        // Constructor to initialize the ExclusiveModePrinter object
        Serial.println(F("Exclusive Mode: "));
    }

    inline void visit(Sensor &sensor) override {}
    inline void visit(SensorConfig &config) override {}
    inline void visit(KnobAxis &axis) override {};
    inline void visit(KnobAxisConfig &config) override {};
    void visit(KinematicsConfig &config) override {
        bool exclusiveMode = config.getExclusiveMode(); // Get the minimum value from the sensor configuration
        Serial.print(exclusiveMode);
        TextHelper::printBooleanDescription(exclusiveMode);
        Serial.println();
    }
};
