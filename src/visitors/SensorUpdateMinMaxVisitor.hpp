#pragma once
#include <common/IVisitor.hpp> // Include the IVisitor interface header file

#include <sensor/sensors/Sensor.hpp>      // For Sensor class
#include <sensor/config/SensorConfig.hpp> // For SensorConfig class
#include <common/esp_print.h>             // For ESP_PRINT

/**
 * @brief   Visitor class for updating the minimum and maximum values of a sensor.
 * @details This class implements the IVisitor interface and is used to visit Sensor objects to update their minimum and maximum values based on the centered value.
 */
class SensorUpdateMinMaxVisitor : public IVisitor {
public:
    SensorUpdateMinMaxVisitor() = default;  // Default constructor
    ~SensorUpdateMinMaxVisitor() = default; // Destructor

    /**
     * @brief   Visit method to update the minimum and maximum values of a Sensor.
     * @param   visitable The VisitableBase object to be visited, which is expected to be a Sensor.
     * @details This method retrieves the centered value from the Sensor and updates the minimum and maximum
     *          values in the SensorConfig if they are lower or higher than the current stored values, respectively.
     *          If the SensorConfig is null, it logs a warning and returns.
     */
    void visit(VisitableBase &visitable) override {

        // Cast the visitable to Sensor
        Sensor &sensor = static_cast<Sensor &>(visitable);

        SensorConfig *sensorcfg = sensor.getConfig();     // Get the sensor configuration to update
        RETURN_W_IF_NULL(sensorcfg, "SensorConfig null"); // Check if the sensor configuration is null

        const int centeredVal = sensor.getCntValue();
        sensorcfg->updateMin(centeredVal); // Update the minimum value in the sensor configuration, if it is lower than the current stored minimum
        sensorcfg->updateMax(centeredVal); // Update the maximum value in the sensor configuration if it is higher than the current stored maximum
    }
};
