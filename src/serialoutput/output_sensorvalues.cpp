#include "output_sensorvalues.h"

#include "hardware/hardware.h"

void Output_SensorValues::update(Hardware *hardware) {
    if (!isDebugOutputDue() || hardware == nullptr) {
        return; // If the debug output is not due, do nothing
    }

    for (uint8_t id = 0; id < MAX_SENSORS; id++) {
        if (id != 0) {
            Serial.print(F(", ")); // Print a comma if it's not the first sensor
        }

        Sensor *sensor = hardware->sensors[id]; // Pointer to the sensor

        Serial.print(sensor->getName()); // Print the sensor name
        Serial.print(F(":"));
        const int value = getSensorValue(sensor); // Get the sensor value using the virtual function
        alignValue(value, 4);                     // Align the value to the right with spaces
        Serial.print(value);
    }
    Serial.println(); // Print a newline after the last sensor status was printed to the serial monitor
}
