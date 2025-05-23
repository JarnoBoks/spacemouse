#pragma once

#include <common/IVisitor.hpp>
#include <sensor/sensors/Sensor.hpp>
#include <common/TextHelper.h>

class IdlePositionPrinter : public IVisitor {
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

    void visit(VisitableBase &visitable) override {
        // Cast the VisitableBase to Sensor
        Sensor &sensor = static_cast<Sensor &>(visitable);

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
};
