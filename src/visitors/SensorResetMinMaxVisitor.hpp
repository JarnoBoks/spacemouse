#pragma once
#include <common/IVisitor.hpp> // Include the IVisitor interface header file

#include <sensor/sensors/Sensor.hpp>      // For Sensor class
#include <sensor/config/SensorConfig.hpp> // For SensorConfig class
#include <common/esp_print.h>             // For ESP_PRINT

/**
 * @brief   Visitor class for resetting the minimum and maximum values of a sensor at the start of a MINMAX calibration.
 */
class SensorResetMinMaxVisitor : public IVisitor {
public:
    SensorResetMinMaxVisitor() = default;  // Default constructor
    ~SensorResetMinMaxVisitor() = default; // Destructor

    /**
     * @brief   Visit method to reset the minimum and maximum values of a Sensor.
     * @param   visitable The VisitableBase object to be visited, which is expected to be a Sensor.
     * @details This method sets the minimum and maximum value to the MAX_INT resp. MIN_INT, in order to prevent that
     *          the current values are limiting the new calibration readings.
     *          F.e. if the current minimum value is -100, and the sensor reads a value of -50, the minimum value would
     *          not be updated to -50, but remain at -100. By setting the minimum to INT_MAX, the new calibration
     *          can start from a clean slate, allowing the new minimum and maximum values to be determined based on the
     *          new readings.
     */
    void visit(VisitableBase &visitable) override {

        // Cast the visitable to Sensor
        Sensor &sensor = static_cast<Sensor &>(visitable);

        SensorConfig *sensorcfg = sensor.getConfig();     // Get the sensor configuration to update
        RETURN_W_IF_NULL(sensorcfg, "SensorConfig null"); // Check if the sensor configuration is null

        sensorcfg->setMin(INT_MAX);
        sensorcfg->setMax(INT_MIN);
    }
};
