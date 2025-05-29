#pragma once
#include <common/IVisitor.hpp> // Include the IVisitor interface header file

#include <sensor/sensors/Sensor.hpp>      // For Sensor class
#include <sensor/config/SensorConfig.hpp> // For SensorConfig class
#include <common/esp_print.h>             // For ESP_PRINT

/**
 * @brief Visitor class for handling exclusive mode for translational or rotational movement.
 * @warning This class should only be used for visiting KinematicsAxis objects.
 * @deprecated This class is not used in the current implementation and may be removed in future versions.
 */
class SensorUpdateMinMaxVisitor : public IVisitor {
    const int *m_minValue = 0;   // Default minimum value
    const int *m_maxValue = 0;   // Default maximum value
    const bool m_persist = true; // Flag to indicate if the configuration should be persisted
public:
    SensorUpdateMinMaxVisitor(const int *min, const int *max, const bool persist)
        : m_minValue(min), m_maxValue(max), m_persist(persist) {}

    ~SensorUpdateMinMaxVisitor() = default; // Destructor

    void visit(VisitableBase &visitable) override {
        if (!&visitable) {
            ESP_WARN("Visitable null");
            return;
        }

        // Cast the VisitableBase to Sensor
        Sensor &sensor = static_cast<Sensor &>(visitable);

        if (!&sensor) {
            ESP_WARN("Sensor null");
            return; // Skip if the sensor is not available
        }
        uint8_t id = sensor.getId(); // Get the sensor ID

        SensorConfig *sensorcfg = sensor.getConfig(); // Get the sensor configuration to update
        if (!sensorcfg) {
            ESP_WARN("SensorConfig null");
            return; // Handle null case gracefully
        }
        sensorcfg->setMin(m_minValue[id]); // Set the minimum value in the sensor configuration
        sensorcfg->setMax(m_maxValue[id]); // Set the maximum value in the sensor configuration

        // Save the sensor configuration to EEPROM
        sensorcfg->persist(sensor.getId()); // Save the updated configuration to EEPROM
    }
};