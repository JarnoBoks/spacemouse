#pragma once

#include <common/IVisitor.hpp>
#include <sensor/sensors/Sensor.hpp>
#include <common/TextHelper.h>

/**
 * @brief Prints the min, mean, max readings and the calculated deadzone for a sensor during
 *        idle calibration.
 * @note This class implements the IVisitor interface and is used to visit Sensors.
 */
class SensorIdleCalibrationResultPrinter : public IVisitor {
private:
    int minValue = 0;     // Minimum read value during Idle calibration
    int maxValue = 0;     // Maximum read value during Idle calibration
    uint8_t deadzone = 0; // Deadzone value during Idle calibration
public:
    /**
     * @brief Constructor for SensorIdleCalibrationResultPrinter
     * @details Initializes the printer and prints the header for the idle position output.
     */
    SensorIdleCalibrationResultPrinter() {
        // Print the header for the idle position output
        Serial.println(F("\n#####  Min | Mean |  Max | Deadzone"));
    }

    inline void setPrintParams(const int min, const int max, const uint8_t dz) {
        minValue = min;
        maxValue = max;
        deadzone = dz;
    }

    /**
     * @brief Visits a sensor and prints its idle position information.
     * @param visitable The sensor to visit.
     */
    void visit(VisitableBase &visitable) override {
        // Cast the VisitableBase to Sensor
        Sensor &sensor = static_cast<Sensor &>(visitable);

        // name
        Serial.print(sensor.getDescriptor());
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
};
