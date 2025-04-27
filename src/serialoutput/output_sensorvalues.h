#ifndef OUTPUT_SENSORVALUES_H
#define OUTPUT_SENSORVALUES_H

#include "idebugmonitor.h"
#include "hardware/hardware.h"
#include "text.h"

enum SensorOutputValType_t {
    SO_RAW = 0,
    SO_CENTERED,
};

/**
 * @brief Class to output raw sensor values for debugging purposes.
 * This class inherits from DebugMonitor.
 *
 * DEBUG_LEVEL = 1 / 2
 */
class Output_SensorValues : public IDebugMonitor {
private:
protected:
    virtual const int getSensorValue(const Sensor *sensor) const = 0;

public:
    Output_SensorValues() {};
    ~Output_SensorValues() {}; // nothing to do in destructor

    void update(const Axis *axis) override {
        // Doing nothing here, as this output class is only used for sensors and not for axes.
    }

    virtual void update(Hardware *hardware) override;
};

class Output_SensorValuesRaw : public Output_SensorValues {
private:
protected:
    inline const int getSensorValue(const Sensor *sensor) const override {
        return sensor->getRawValue(); // Get the raw value from the sensor
    }
};

class Output_SensorValuesCentered : public Output_SensorValues {
private:
protected:
    inline const int getSensorValue(const Sensor *sensor) const override {
        return sensor->getCenteredValue(); // Get the centered value from the sensor
    }
};

class Output_SensorValuesFiltered : public Output_SensorValues {
private:
protected:
    inline const int getSensorValue(const Sensor *sensor) const override {
        return sensor->getFilteredValue(); // Get the filtered value from the sensor
    }
};

#endif // OUTPUT_ENSORVALUES_H