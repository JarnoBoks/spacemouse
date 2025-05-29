#pragma once

#include <common/IVisitor.hpp>
#include <sensor/sensors/Sensor.hpp>
#include <sensor/config/SensorConfig.hpp>
#include <common/TextHelper.h>
#include <common/esp_print.h> // For ESP_PRINT

// REFACTOR - Warninglevel should be set in the SensorConfig class, not here
#define SIDP_DEADZONEWARNING 10 // Define a threshold for dead zone warning

class SensorIdleDeadzonePrinter : public IVisitor {
public:
    SensorIdleDeadzonePrinter() {
        // Constructor to initialize the DeadzonePrinter object
        // Print the header for the deadzone output
        Serial.println(F("        Idle |  DZ  | Warning"));
    }

    /**
     * @brief Visit the Sensor and output the Sensor IdlePosition and deadzone values.
     * @param config The SensorConfig object to visit.
     */
    void visit(VisitableBase &visitable) override {
        if (&visitable == nullptr) {
            ESP_WARN("Visitable null");
            return; // Handle null case gracefully
        }

        // Cast the VisitableBase to Sensor
        Sensor &sensor = static_cast<Sensor &>(visitable);

        SensorConfig *config = sensor.getConfig();
        if (!config) {
            ESP_WARN("SensorConfig null");
            return; // Handle null case gracefully
        }
        // Print the sensor name
        Serial.print(sensor.getDescriptor());
        Serial.print(F(": "));
        // Print the sensor idle position
        TextHelper::alignedPrint(sensor.getIdlePosition(), 4); // Align the value to the right with spaces
        TextHelper::printSeparator();                          // Print a separator between values

        // Print the deadzone value
        TextHelper::alignedPrint(config->getDeadzone(), 4); // Align the value to the right with spaces
        TextHelper::printSeparator();                       // Print a separator between values

        // Check if the deadzone is set to a warning value
        if (config->getDeadzone() > SIDP_DEADZONEWARNING) {
            Serial.print(F("Warning")); // Print warning if the deadzone is above the threshold
        }

        Serial.println(); // Print a newline after the output
    }
};
