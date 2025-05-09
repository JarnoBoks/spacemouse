#include "DebugOutputSensors.hpp"
#include "sensor/SensorCollection.hpp"
#include "sensor/sensors/Sensor.hpp"
#include "common/TextHelper.h"

void DebugOutputSensors::update(SensorCollection *sensorCollection) {
    if (!isDebugOutputDue() || sensorCollection == nullptr) {
        return; // If the debug output is not due, do nothing
    }

    for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {
        TextHelper::printLeadingComma(id); // Print a komma if it's not the first sensor

        Sensor *sensor = sensorCollection->getSensor(id); // Pointer to the sensor

        Serial.print(sensor->getName()); // Print the sensor name
        Serial.print(F(":"));
        const int value = getSensorValue(sensor); // Get the sensor value using the virtual function
        TextHelper::alignValue(value, 4);         // Align the value to the right with spaces
        Serial.print(value);
    }
}
