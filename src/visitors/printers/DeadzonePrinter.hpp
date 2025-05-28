#pragma once

#include <common/IVisitor.hpp>
#include <sensor/config/SensorConfig.hpp>
#include <common/TextHelper.h>
#include <common/esp_print.h> // For ESP_PRINT

class DeadzonePrinter : public IVisitor {
public:
    DeadzonePrinter() {
        // Constructor to initialize the DeadzonePrinter object
        // Print the header for the deadzone output
        Serial.println(F("        Deadzone | Warning"));
    }

    /**
     * @brief Visit the SensorConfig object and print its deadzone values.
     * @param config The SensorConfig object to visit.
     */
    void visit(VisitableBase &visitable) override {
        if (&visitable == nullptr) {
            ESP_WARN("Visitable null");
            return; // Handle null case gracefully
        }

        // Cast the VisitableBase to SensorConfig
        SensorConfig &config = static_cast<SensorConfig &>(visitable);

        // Retrieve the sensor deadzone value
        const uint8_t deadzone = config.getDeadzone();

        TextHelper::alignValue(deadzone, 4);
        Serial.print(deadzone);

        Serial.println();
    }
};
