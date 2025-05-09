#pragma once

#include "IPrinterVisitor.h"
#include "sensor/sensor.hpp"
#include "sensor/config/SensorConfig.h"
#include "hardware/hardware.h"
#include "kinematics/kinematicsconfig.h"
#include "common/TextHelper.h"

class IdlePositionPrinter : public IPrinterVisitor {
private:
    int minValue = 0; // Minimum read value during Idle calibration
    int maxValue = 0; // Maximum read value during Idle calibration
    int deadzone = 0; // Deadzone value during Idle calibration
public:
    IdlePositionPrinter() {
        // Constructor to initialize the MinMaxPrinter object
        // Print the header for the idle position output
        Serial.println(F("\n#####  Min | Mean |  Max | Deadzone"));
    }

    inline void setPrintParams(const int min, const int max, const int dz) {
        minValue = min; // Set the minimum value
        maxValue = max; // Set the maximum value
        deadzone = dz;  // Set the deadzone value
    }

    void visit(Sensor &sensor) override {
        // name
        Serial.print(sensor.getName());
        Serial.print(F(": "));

        // min
        TextHelper::alignedPrint(minValue, 4); // Align the value to the right with spaces
        TextHelper::printSeparator();          // Print a separator between values

        // idle
        TextHelper::alignedPrint(sensor.getIdlePosition(), 4); // Align the value to the right with spaces
        TextHelper::printSeparator();                          // Print a separator between values

        // max
        TextHelper::alignedPrint(maxValue, 4);
        TextHelper::printSeparator(); // Print a separator between values

        // deadzone
        TextHelper::alignedPrint(deadzone, 4); // Align the value to the right with spaces
        Serial.print(F("   "));

        Serial.println();
    }

    inline void visit(SensorConfig &config) override {}
    inline void visit(Hardware &hardware) override {}
    inline void visit(Axis &axis) override {};
    inline void visit(AxisConfig &config) override {};
    inline void visit(KinematicsConfig &config) override {}
};
