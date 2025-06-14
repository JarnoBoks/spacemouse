#pragma once

#include <common/IVisitor.hpp>
#include <sensor/sensors/Sensor.hpp>
#include <sensor/config/SensorConfig.hpp>

#include <common/TextHelper.h>
#include <common/esp_print.h> // For ESP_PRINT

/**
 * @brief Visitor class to print the min, max, and range values of a SensorConfig object.
 * @details This class implements the IVisitor interface and is used to visit SensorCollection or Sensor instances.
 */
class SensorMinMaxPrinter : public IVisitor {
private:
    bool m_warningsOccurred = false; // Flag to indicate if any warnings occurred during the visit
public:
    /**
     * @brief Construct a new SensorMinMaxPrinter object.
     * @details This constructor initializes the printer and prints the header for the min/max output.
     */
    SensorMinMaxPrinter() {
        // Constructor to initialize the MinMaxPrinter object
        // Print the header for the min/max output
        Serial.println(F("\n       Min |  Max | Rnge | Warning"));
    }

    inline bool hasWarningsOccurred() const { return m_warningsOccurred; }

    /**
     * @brief Visit the Sensor and print its configuration values.
     */
    void visit(VisitableBase &visitableSensor) override {

        // Cast the VisitableBase to Sensor
        Sensor &sensor = static_cast<Sensor &>(visitableSensor);

        // Initialize the flags for min-, max- and workingrange warnings
        bool warningsOccurred = false, minWarning = false, maxWarning = false, rangeWarning = false;

        SensorConfig *config = sensor.getConfig();
        if (!config) {
            ESP_WARN("SensorConfig null");
            return;
        }

        // Retrieve the sensor configuration values & warningstate
        const int min = config->getMin(&minWarning);
        const int max = config->getMax(&maxWarning);
        const int range = config->getRange(&rangeWarning);

        // Print the sensor descriptor and the min, max, and range values
        Serial.print(sensor.getDescriptor());
        Serial.print(F(": "));
        TextHelper::alignValue(min, 4);
        Serial.print(min);
        TextHelper::printSeparator();
        TextHelper::alignValue(max, 4);
        Serial.print(max);
        TextHelper::printSeparator();
        TextHelper::alignValue(range, 4);
        Serial.print(range); // Print the working range

        TextHelper::printSeparator();

        // Print the warning status for min, max and working range
        if (minWarning) {
            Serial.print(F("Min"));
        }
        warningsOccurred = warningsOccurred || minWarning;

        if (maxWarning) {
            TextHelper::printLeadingComma(warningsOccurred); // Print a comma if there where other warnings before
            Serial.print(F("Max"));
        }
        warningsOccurred = warningsOccurred || maxWarning;

        if (rangeWarning) {
            TextHelper::printLeadingComma(warningsOccurred);
            Serial.print(F("Range"));
        }
        warningsOccurred = warningsOccurred || rangeWarning; // Set the warning status if any of the conditions are met

        if (minWarning || maxWarning || rangeWarning) {
            Serial.print(F(" small"));
        } else {
            Serial.print(F("-"));
        }

        Serial.println();

        m_warningsOccurred = m_warningsOccurred || warningsOccurred; // Update the overall warning status
    }
};
