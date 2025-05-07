#pragma once

#include "IPrinterVisitor.h"
#include "sensor/sensor.h"
#include "sensor/sensorconfig.h"
#include "hardware/hardware.h"
#include "kinematics/kinematicsconfig.h"
#include "common/TextHelper.h"

class SwitchYZPrinter : public IPrinterVisitor {
public:
    SwitchYZPrinter() {
        // Constructor to initialize the SwitchYZPrinter object
        Serial.println(F("SwitchYZ: "));
    }

    inline void visit(Sensor &sensor) override {}
    inline void visit(SensorConfig &config) override {}
    inline void visit(Hardware &hardware) override {}
    inline void visit(Axis &axis) override {};
    inline void visit(AxisConfig &config) override {};

    inline void visit(KinematicsConfig &config) override {
        bool switchYZ = config.switchYZ; // Get the minimum value from the sensor configuration
        Serial.print(switchYZ);
        TextHelper::printBooleanDescription(switchYZ);
        Serial.println();
    }
};
