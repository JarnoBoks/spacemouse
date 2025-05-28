#pragma once

#include <common/IVisitor.hpp>
#include <sensor/config/SensorConfig.hpp>
#include <common/TextHelper.h>
#include <common/esp_print.h> // For ESP_PRINT

class MinMaxPrinter : public IVisitor {
public:
    MinMaxPrinter() {
        // Constructor to initialize the MinMaxPrinter object
        // Print the header for the min/max output
        Serial.println(F("        Min |  Max | Rnge | Warning"));
    }

    /**
     * @brief Visit the SensorConfig object and print its configuration values.
     * @param config The SensorConfig object to visit.
     */
    void visit(VisitableBase &visitable) override {
        // REFACTOR - The check has to be implented in all visitors
        if (!&visitable) {
            ESP_WARN("Visitable null");
            return;
        }

        // Cast the VisitableBase to SensorConfig
        SensorConfig &config = static_cast<SensorConfig &>(visitable);

        // Initialize the flags for min-, max- and workingrange warnings
        bool warningsOccurred = false, minWarning = false, maxWarning = false, rangeWarning = false;

        // Retrieve the sensor configuration values & warningstate
        const int min = config.getMin(&minWarning);
        const int max = config.getMax(&maxWarning);
        const int range = config.getRange(&rangeWarning);

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
};
