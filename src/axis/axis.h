#ifndef AXIS_H
#define AXIS_H

#include "axis/axistype.h"       // For AxisType_t
#include "modfunctype.h"         // For ModFunc_t
#include "observers/IObserver.h" // For MAX_AXIS_OBSERVERS

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

    /// @brief
    int16_t value = 0; // The value of the axis after reading from the hardware and applying all axis & kinematics configurations.

    /// @brief The value of the axis after reading from the hardware and applying the sensitivity configuration.
    /// @details This value is stored for debugging purposes.
    int16_t modifiedValue = 0; // The value of the axis after reading from the hardware and just applying the sensiti

    /// @brief The value of the axis after reading from the hardware and applying the sensitivity configuration.
    /// @details This value is stored for debugging purposes.
    int16_t rawValue = 0;

    AxisConfig *config = nullptr;
    LightBehavior *light = nullptr;

    void modifier(ModFunc_t type);   // Modifier function for this axis & direction
    bool isKillSwitchActive = false; // Flag to indicate if the kill switch for this Axis is active

public:
    Axis();                // Default constructor (not used in the code)
    Axis(AxisType_t type); // Constructor with axis type

    void setKillSwitchActive(bool active) { isKillSwitchActive = active; } // Setter for kill switch state

    inline int16_t getValue() const { return value; }                 // Getter for value           // REVIEW - Same asa Sensor class, but we need to check if we can use the same function for both classes.
    inline int16_t getRawValue() const { return rawValue; }           // Getter for rawValue
    inline int16_t getModifiedValue() const { return modifiedValue; } // Getter for modifiedValue
    inline const char *getName() const { return name; }               // Getter for name            // REVIEW - Same as Sensor class, but we need to check if we can use the same function for both classes.
    inline AxisType_t getType() const { return type; }                // Getter for type
    inline AxisConfig *getConfig() const { return config; }           // Getter for config

    inline void setValue(int16_t value) { this->value = value; } // Setter for value (used by kinematics, for Exclusieve mode & YZ switching)
    void calculateValue(int16_t hwvalue);                        // Calculate the value of the axis based on the hardware input and the configuration

    void accept(IPrinterVisitor &visitor);
};

#endif // AXIS_H