#include "DebugOutputSensors.hpp"
#include "hardware/hardware.h"
#include "common/TextHelper.h"

// REVIEW - Move to Visitor?

void DebugOutputSensors::update(Hardware *hardware) {
    if (!isDebugOutputDue() || hardware == nullptr) {
        return; // If the debug output is not due, do nothing
    }

    for (uint8_t id = 0; id < MAX_SENSORS; id++) {
        TextHelper::printLeadingComma(id); // Print a komma if it's not the first sensor

        Sensor *sensor = hardware->sensors[id]; // Pointer to the sensor

        Serial.print(sensor->getName()); // Print the sensor name
        Serial.print(F(":"));
        const int value = getSensorValue(sensor); // Get the sensor value using the virtual function
        TextHelper::alignValue(value, 4);         // Align the value to the right with spaces
        Serial.print(value);
    }
}
