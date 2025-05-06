#pragma once

#include "ObserverDebugOutput.hpp"
#include "kinematics/kinematics.h"

class Output_AxisValues : public ObserverDebugOutput {
private:
protected:
    virtual const int getAxisValue(const Axis *axis) const = 0;

public:
    Output_AxisValues() = default;
    virtual ~Output_AxisValues() {};

    void update(Kinematics *kinematics) override;
};

/**
 * @brief Output class for raw axis values.
 * @details This class inherits from Output_AxisValues and overrides the getAxisValue method to return the raw value of the axis.
 * It also overrides the update method to print the axis values to the serial monitor.
 */
class Output_AxisValuesRaw : public Output_AxisValues {
private:
protected:
    inline const int getAxisValue(const Axis *axis) const override {
        return axis->getRawValue(); // Get the axis value
    }

public:
    inline void update(Kinematics *kinematics) override {
        Output_AxisValues::update(kinematics); // Call the base class update method
        Serial.println();
    }
};

class Output_AxisValuesModified : public Output_AxisValues {
private:
protected:
    inline const int getAxisValue(const Axis *axis) const override {
        return axis->getModifiedValue(); // Get the axis value
    }

public:
    inline void update(Kinematics *kinematics) override {
        Output_AxisValues::update(kinematics); // Call the base class update method
        Serial.println();
    }
};

class Output_AxisValuesModifiedWithoutNewline : public Output_AxisValues {
private:
protected:
    inline const int getAxisValue(const Axis *axis) const override {
        return axis->getModifiedValue(); // Get the axis value
    }
};