#ifndef AXIS_H
#define AXIS_H

#include <Arduino.h>
#include "observers/IObserver.h" // For MAX_AXIS_OBSERVERS
#include "modifierfunction.h"    // For ModFunc_t

enum AxisType_t : int8_t { ALL = -2,
                           UNINITIALIZED = -1,
                           TRANSX = 0,
                           TRANSY,
                           TRANSZ,
                           ROTX,
                           ROTY,
                           ROTZ,
                           LENGTH };

#define AXIS_NAMES {"TX", "TY", "TZ", "RX", "RY", "RZ"} // Axis names for debugging

// Forward declaration of classes to avoid circular dependencies
class AxisConfig;
class Hardware;
class LightBehavior;
class IPrinterVisitor;

class Axis {
private:
    AxisType_t type = UNINITIALIZED;
    const char *name;

    /// @brief The value of the axis after reading from the hardware and applying all axis & kinematics configurations.
    int16_t value = 0;

    /// @brief The value of the axis after reading from the hardware and applying the sensitivity configuration.
    /// @details This value is stored for debugging purposes.
    int16_t modifiedValue = 0;

    /// @brief The value of the axis after reading from the hardware and applying the sensitivity configuration.
    /// @details This value is stored for debugging purposes.
    int16_t rawValue = 0;

    AxisConfig *config = nullptr;
    Hardware *hardware = nullptr;
    LightBehavior *light = nullptr;
    uint8_t observerCount = 0;

    void modifier(ModFunc_t type); // Modifier function for this axis & direction

public:
    Axis(); // Default constructor
    Axis(AxisType_t type);

    inline int16_t getValue() const { return value; }            // Getter for value           // REVIEW - Same asa Sensor class, but we need to check if we can use the same function for both classes.
    inline void setValue(int16_t value) { this->value = value; } // Setter for value (used by kinematics, for Exclusieve mode & YZ switching)

    inline int16_t getRawValue() const { return rawValue; }           // Getter for rawValue
    inline int16_t getModifiedValue() const { return modifiedValue; } // Getter for modifiedValue

    inline const char *getName() const { return name; } // Getter for name            // REVIEW - Same as Sensor class, but we need to check if we can use the same function for both classes.

    void calculateValue();

    void setLedLight(LightBehavior *behavior);

    inline AxisType_t getType() const { return type; } // Getter for type

    inline AxisConfig *getConfig() const { return config; } // Getter for config

    void accept(IPrinterVisitor &visitor);
};

#endif // AXIS_H