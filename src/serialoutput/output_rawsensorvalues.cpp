#include "output_rawsensorvalues.h"

#include "hardware/hardware.h"

void Output_RawSensorValues::update(Hardware *hardware) {
    if (!isDebugOutputDue() || hardware == nullptr) {
        return; // If the debug output is not due, do nothing
    }

    Sensor *sensors[MAX_SENSORS];  // Array to hold sensor pointers
    hardware->getSensors(sensors); // Get the sensors from the hardware

    for (uint8_t id = 0; id < MAX_SENSORS; id++) {
        if (id != 0) {
            Serial.print(F(", ")); // Print a comma if it's not the first sensor
        }

        // Output the raw value of the sensor
        Serial.print(sensors[id]->getName());
        Serial.print(F(": "));
        const int value = sensors[id]->getRawValue();
        alignValue(value, 4); // Align the value to the right with spaces
        Serial.print(value);
    }

    Serial.println(); // Print a newline after the last sensor
}
