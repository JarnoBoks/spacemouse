#pragma once

#include "IPrinterVisitor.h"
#include "kinematics/config/kinematicsconfig.hpp"
#include "common/TextHelper.h"

class SwitchYZPrinter : public IPrinterVisitor {
public:
    SwitchYZPrinter() {
        // Constructor to initialize the SwitchYZPrinter object
        Serial.println(F("SwitchYZ: "));
    }

    inline void visit(Sensor &sensor) override {}
    inline void visit(SensorConfig &config) override {}
    inline void visit(KnobMotionVector &axis) override {};
    inline void visit(KnobVectorConfig &config) override {};

    void visit(KinematicsConfig &config) override {
        bool switchYZ = config.getSwitchYZ(); // Get the YZ switching state from the kinematics configuration
        Serial.print(switchYZ);
        TextHelper::printBooleanDescription(switchYZ);
        Serial.println();
    }
};
