#pragma once
#include <common/IVisitor.hpp> // Include the IVisitor interface header file

#include <sensor/sensors/Sensor.hpp>      // For Sensor class
#include <sensor/config/SensorConfig.hpp> // For SensorConfig class
#include <common/esp_print.h>             // For ESP_PRINT

/**
 * @brief Visitor class for updating the minimum and maximum values of a sensor.
 * @details This class implements the IVisitor interface and is used to visit Sensor objects to update their minimum and maximum values based on the centered value.
 *          It checks if the sensor and its configuration are valid before performing the update.
 * @note The visit method retrieves the sensor ID, gets the sensor configuration, and updates the minimum and maximum values if they are lower or higher than the current values.
 */
class SensorUpdateMinMaxVisitor : public IVisitor {
public:
    SensorUpdateMinMaxVisitor() = default;  // Default constructor
    ~SensorUpdateMinMaxVisitor() = default; // Destructor

    void visit(VisitableBase &visitable) override {
        RETURN_W_IF_NULL(&visitable, "Visitable null"); // Check if the visitable is null

        // Cast the visitable to Sensor
        Sensor &sensor = static_cast<Sensor &>(visitable);
        RETURN_W_IF_NULL(&sensor, "Sensor null"); // Check if the sensor is null

        uint8_t id = sensor.getId(); // Get the sensor ID

        SensorConfig *sensorcfg = sensor.getConfig();     // Get the sensor configuration to update
        RETURN_W_IF_NULL(sensorcfg, "SensorConfig null"); // Check if the sensor configuration is null

        const int centeredVal = sensor.getCntValue();
        sensorcfg->updateMin(centeredVal); // Update the minimum value in the sensor configuration if it is lower than the current minimum
        sensorcfg->updateMax(centeredVal); // Update the maximum value in the sensor configuration if it is higher than the current maximum
    }
};
