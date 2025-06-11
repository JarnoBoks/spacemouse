#pragma once

#include <common/IVisitor.hpp>
#include <sensor/sensors/Sensor.hpp>
#include <sensor/config/SensorConfig.hpp>
#include <common/TextHelper.h>
#include <common/esp_print.h> // For ESP_PRINT

class SensorIdleDeadzonePrinter : public IVisitor {
public:
    SensorIdleDeadzonePrinter() {
        // Constructor to initialize the DeadzonePrinter object
        // Print the header for the deadzone output
        Serial.println(F("      Idle |  DZ  | Warning"));
    }

    /**
     * @brief Visit the Sensor and output the Sensor IdlePosition and deadzone values.
     * @details This function prints the sensor's idle position and deadzone values, along with a warning if the deadzone exceeds a predefined threshold.
     */
    void visit(VisitableBase &visitableSensor) override {

        // Cast the VisitableBase to Sensor
        Sensor &sensor = static_cast<Sensor &>(visitableSensor);

        // Print the sensor name
        Serial.print(sensor.getDescriptor());
        Serial.print(F(": "));
        // Print the sensor idle position
        TextHelper::alignedPrint(sensor.getIdlePosition(), 4); // Align the value to the right with spaces
        TextHelper::printSeparator();                          // Print a separator between values

        // Print the deadzone value
        TextHelper::alignedPrint(sensor.getDeadzone(), 4); // Align the value to the right with spaces
        TextHelper::printSeparator();                      // Print a separator between values

        if (!sensor.isDeadzoneOk()) {
            Serial.print(F("Warning")); // Print warning if the deadzone is above the threshold
        } else {
            Serial.print(F("-")); // Print spaces if no warning
        }
        Serial.println(); // Print a newline after the output
    }
};
