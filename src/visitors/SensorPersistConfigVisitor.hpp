#pragma once
#include <common/IVisitor.hpp> // Include the IVisitor interface header file

#include <sensor/sensors/Sensor.hpp>      // For Sensor class
#include <sensor/config/SensorConfig.hpp> // For SensorConfig class
#include <common/esp_print.h>             // For ESP_PRINT

/**
 * @brief   Visitor class for updating the minimum and maximum values of a sensor.
 * @details This class visits Sensor objects to persist the sensor configuration to the non-volatile storage.
 */
class SensorPersistConfigVisitor : public IVisitor {
public:
    SensorPersistConfigVisitor() = default;  // Default constructor
    ~SensorPersistConfigVisitor() = default; // Destructor

    void visit(VisitableBase &visitableSensor) override {

        // Cast the visitable to Sensor
        Sensor &sensor = static_cast<Sensor &>(visitableSensor);

        SensorConfig *sensorcfg = sensor.getConfig();
        RETURN_W_IF_NULL(sensorcfg, "SensorConfig null");

        sensorcfg->persist();
    }
};
