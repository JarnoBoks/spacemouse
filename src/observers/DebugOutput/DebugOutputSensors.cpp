#include "DebugOutputSensors.hpp"
#include "sensor/SensorCollection.hpp"
#include "sensor/sensors/Sensor.hpp"
#include "common/TextHelper.h"

#include <Arduino.h> // For Serial

// void DebugOutputSensors::update(SensorCollection *sensorCollection) {
void DebugOutputSensors::update(IObservable *sensorCollection) {

    // Check if the debug output is due and if the sensor collection is not null
    if (!isDebugOutputDue() || !sensorCollection) {
        return;
    }

    // DEVNOTE - The Observable is a SensorCollection. Cast is necessary to access Collection functions.
    for (uint8_t id = 0; id < static_cast<SensorCollection *>(sensorCollection)->getItemCount(); id++) {
        TextHelper::printLeadingComma(id); // Print a komma if it's not the first sensor

        Sensor *sensor = static_cast<SensorCollection *>(sensorCollection)->getSensor(id); // Pointer to the sensor

        Serial.print(sensor->getDescriptor()); // Print the sensor name
        Serial.print(F(":"));
        const int value = getSensorValue(sensor); // Get the sensor value using the virtual function
        TextHelper::alignValue(value, 4);         // Align the value to the right with spaces
        Serial.print(value);
    }

    endOutput(); // End the debug output, e.g., by printing a new line or space
}
