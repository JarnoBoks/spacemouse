#pragma once
#ifndef MINMAXPRINTER_H
#define MINMAXPRINTER_H

#include "IPrinterVisitor.h"
#include "sensor/sensor.h"
#include "sensor/sensorconfig.h"
#include "hardware/hardware.h"
#include "text.h"

class MinMaxPrinter : public IPrinterVisitor {
public:
    MinMaxPrinter() {
        // Constructor to initialize the MinMaxPrinter object
        // Print the header for the min/max output
        Serial.println(F("        Min |  Max | Range | Warning"));
    }

    inline void visit(Sensor &sensor) override {
        Serial.print(sensor.getName());
        Serial.print(F(":  "));
    }

    inline void visit(SensorConfig &config) override {
        // Initialize the flags for min, max and working range warnings
        bool warningsOccurred = false, minWarning = false, maxWarning = false, rangeWarning = false;

        // Retrieve the sensor configuration values & warningstate
        int min = config.getMin(&minWarning);       // Get the minimum value from the sensor configuration
        int max = config.getMax(&maxWarning);       // Get the maximum value from the sensor configuration
        int range = config.getRange(&rangeWarning); // Get the working range from the sensor configuration

        TextHelper::alignValue(min, 4);
        Serial.print(min);
        TextHelper::printSeparator(); // Print the ' | ' separator
        TextHelper::alignValue(max, 4);
        Serial.print(max);
        TextHelper::printSeparator();
        TextHelper::alignValue(range, 4);
        Serial.print(range); // Print the working range

        // TODO - Program more efficiently
        // Print the warning status for min, max and working range
        if (minWarning) {
            Serial.print(F("Min "));
        }
        warningsOccurred = warningsOccurred || minWarning; // Set the warning status if any of the conditions are met
        if (maxWarning) {
            if (warningsOccurred)
                Serial.print(F(", "));

            Serial.print(F("Max "));
        }
        warningsOccurred = warningsOccurred || maxWarning; // Set the warning status if any of the conditions are met

        if (rangeWarning) {
            if (warningsOccurred)
                Serial.print(F(", "));

            Serial.print(F("Range "));
        }
        warningsOccurred = warningsOccurred || rangeWarning; // Set the warning status if any of the conditions are met

        if (warningsOccurred) {
            Serial.print(F("small"));
        } else {
            Serial.print(F("ok"));
        }

        Serial.println();
    }

    inline void visit(Hardware &hardware) override {}
};

#endif // MINMAXPRINTER_H