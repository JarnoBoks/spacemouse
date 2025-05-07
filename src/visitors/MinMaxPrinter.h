#pragma once
#ifndef MINMAXPRINTER_H
#define MINMAXPRINTER_H

#include "IPrinterVisitor.h"
#include "sensor/sensor.h"
#include "sensor/sensorconfig.h"
#include "hardware/hardware.h"
#include "kinematics/kinematicsconfig.h"
#include "common/TextHelper.h"

class MinMaxPrinter : public IPrinterVisitor {
public:
    MinMaxPrinter() {
        // Constructor to initialize the MinMaxPrinter object
        // Print the header for the min/max output
        Serial.println(F("        Min |  Max | Rnge | Warning"));
    }

    inline void visit(Sensor &sensor) override {
        Serial.print(sensor.getName());
        Serial.print(F(":  "));
    }

    inline void visit(SensorConfig &config) override {
        // Initialize the flags for min-, max- and workingrange warnings
        bool warningsOccurred = false, minWarning = false, maxWarning = false, rangeWarning = false;

        // Retrieve the sensor configuration values & warningstate
        int min = config.getMin(&minWarning);
        int max = config.getMax(&maxWarning);
        int range = config.getRange(&rangeWarning);

        TextHelper::alignValue(min, 4);
        Serial.print(min);
        TextHelper::printSeparator();
        TextHelper::alignValue(max, 4);
        Serial.print(max);
        TextHelper::printSeparator();
        TextHelper::alignValue(range, 4);
        Serial.print(range); // Print the working range

        TextHelper::printSeparator();

        // TODO - Program more efficiently
        // Print the warning status for min, max and working range
        if (minWarning) {
            Serial.print(F("Min"));
        }
        warningsOccurred = warningsOccurred || minWarning; // Set the warning status if any of the conditions are met

        if (maxWarning) {
            TextHelper::printLeadingComma(warningsOccurred); // Print a comma if there where other warnings before
            Serial.print(F("Max"));
        }
        warningsOccurred = warningsOccurred || maxWarning; // Set the warning status if any of the conditions are met

        if (rangeWarning) {
            TextHelper::printLeadingComma(warningsOccurred);
            Serial.print(F("Range")); // Corrected to use CF(t_Range) without the cursor
        }
        warningsOccurred = warningsOccurred || rangeWarning; // Set the warning status if any of the conditions are met

        if (warningsOccurred) {
            Serial.print(F(" small"));
        } else {
            Serial.print(F("-"));
        }

        Serial.println();
    }

    inline void visit(Hardware &hardware) override {}
    inline void visit(Axis &axis) override {};
    inline void visit(AxisConfig &config) override {};
    inline void visit(KinematicsConfig &config) override {}
};

#endif // MINMAXPRINTER_H