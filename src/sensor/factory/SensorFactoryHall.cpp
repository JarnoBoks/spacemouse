#include "SensorFactoryHall.hpp" // Include the header file for the sensor factory

// Include header file for the key to create
#include <sensor/sensors/HallSensor.hpp>

// Include the configuration header file for the key configuration in this factory
#include "config.h"

#include <common/esp_print.h> // For ESP_ERROR() and ESP_DEBUG() macros

#include <pins_arduino.h> // For the pin definitions
#include <binary.h>       // For the binary operations

#define HALL_SENSOR_PINLIST PINLIST // Pins for the sensors, as defined in config.h

Sensor *SensorFactoryHall::create(uint8_t id) {
    if (id >= HALL_LENGTH) {
        ESP_ERROR(F("Invalid Hall sensor id")); // Log an error if the id is invalid
        return nullptr;                         // Return null for invalid id
    }

    // Get the pinNumber from the config.h object for this sensor
    constexpr uint8_t sensorPins[HALL_LENGTH] = HALL_SENSOR_PINLIST; // Pins for the sensors, as defined in config.h

    Sensor *newSensor = new HallSensor(sensorPins[id], static_cast<HallSensorsId_t>(id)); // Create a new hall sensor instance
    return newSensor;                                                                     // Return the new Hall sensor instance
}
