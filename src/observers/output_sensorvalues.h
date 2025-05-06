#ifndef OUTPUT_SENSORVALUES_H
#define OUTPUT_SENSORVALUES_H

#include "ObserverDebugOutput.hpp"
#include "hardware/hardware.h"
#include "TextHelper.h"

enum SensorOutputValType_t {
    SO_RAW = 0,
    SO_CENTERED,
};

class Output_SensorValues : public ObserverDebugOutput {
private:
protected:
    virtual const int getSensorValue(const Sensor *sensor) const = 0;

public:
    Output_SensorValues() {};
    virtual ~Output_SensorValues() {};

    virtual void update(Hardware *hardware) override;
};

/**
 * @brief Output class for raw sensor values.
 * @details This class inherits from Output_SensorValues and overrides the getSensorValue method to return the raw value of the sensor.
 * It also overrides the update method to print the sensor values to the serial monitor.
 */
class Output_SensorValuesRaw : public Output_SensorValues {
private:
protected:
    inline const int getSensorValue(const Sensor *sensor) const override {
        return sensor->getRawValue(); // Get the raw value from the sensor
    }

public:
    inline void update(Hardware *hardware) override {
        Output_SensorValues::update(hardware); // Call the base class update method
        Serial.println();
    }
};

/**
 * @brief Output class for centered sensor values.
 * @details This class inherits from Output_SensorValues and overrides the getSensorValue method to return the centered value of the sensor.
 * It also overrides the update method to print the sensor values to the serial monitor.
 */
class Output_SensorValuesCentered : public Output_SensorValues {
private:
protected:
    inline const int getSensorValue(const Sensor *sensor) const override {
        return sensor->getCenteredValue(); // Get the centered value from the sensor
    }

public:
    inline void update(Hardware *hardware) override {
        Output_SensorValues::update(hardware); // Call the base class update method
        Serial.println();
    }
};

/**
 * @brief Output class for centered sensor values, without a newline (but with a separator).
 * @details This class inherits from Output_SensorValues and overrides the getSensorValue method to return the centered value of the sensor.
 * It also overrides the update method to print the sensor values to the serial monitor.
 * @details This class is used to print the sensor values without a newline at the end, but with a separator. Used in conjunction with the axis values.
 */
class Output_SensorValuesCenteredWithoutNewline : public Output_SensorValues {
private:
protected:
    inline const int getSensorValue(const Sensor *sensor) const override {
        return sensor->getCenteredValue(); // Get the centered value from the sensor
    }

public:
    inline void update(Hardware *hardware) override {
        Output_SensorValues::update(hardware); // Call the base class update method
        TextHelper::printSeparator();          // Print a separator after the sensors values
    }
};

/**
 * @brief Output class for filtered sensor values.
 * @details This class inherits from Output_SensorValues and overrides the getSensorValue method to return the filtered value of the sensor.
 * It also overrides the update method to print the sensor values to the serial monitor.
 */
class Output_SensorValuesFiltered : public Output_SensorValues {
private:
protected:
    inline const int getSensorValue(const Sensor *sensor) const override {
        return sensor->getFilteredValue(); // Get the filtered value from the sensor
    }

public:
    inline void update(Hardware *hardware) override {
        Output_SensorValues::update(hardware); // Call the base class update method
        Serial.println();
    }
};

#endif // OUTPUT_ENSORVALUES_H