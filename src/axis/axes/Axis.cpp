#include "Axis.hpp"
#include "axis/config/AxisConfig.hpp"
#include "visitors/IPrinterVisitor.h"
#include "hardware/hardware.h"

#include <math.h>
#define sign(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0)) // Define Signum Function

// The Axis instantiates the hardware class and the AxisConfig class.
// The AxisConfig class is used to configure the axis, including the sensitivity, gate, and function type.
// The Axis class is used to calculate the value of the axis based on the hardware input and the configuration.
// The hardware is instantiated in the constructor of the Axis class, picking the hardware that is configured in config.h.
Axis::Axis() : type(AxisType_t::UNINITIALIZED) {
    // Default constructor initializes the axis to UNINITIALIZED
    config = new AxisConfig(); // Create a new AxisConfig object for this axis

    light = nullptr;
    value = 0;
}

Axis::Axis(AxisType_t type) : type(type) {
    // Initialize the axis with the given type
    config = new AxisConfig(type); // Create a new AxisConfig object for this axis

    // Setup the name of the axis based on the type
    const char *names[static_cast<int>(AxisType_t::LENGTH)] = AXIS_NAMES;
    if (type <= AxisType_t::UNINITIALIZED || type >= AxisType_t::LENGTH) {
        this->name = "?"; // Set name to UNKNOWN if type is invalid
    } else {
        this->name = names[type];
    }

    light = nullptr;
    value = 0;
}

const bool Axis::isCurrentAxis(const char *name) const {
    if (this->name != nullptr) {
        return (strcmp(this->name, name) == 0); // Compare the name of the axis with the provided name
    }
    return false; // Return false if the name is not set or does not match
}

void Axis::calculateValue(int16_t hwvalue) {
    value = hwvalue; // Set the value to the hardware value

    if (isKillSwitchActive) {
        value = 0; // Set the value to 0 for the kill switch
        return;
    }

    AxisDirectionConfig *dconfig = (value > 0) ? &this->config->posConfig : &this->config->negConfig; // Get the config for the current axis and direction

    // Apply the sensitivity for this axis & direction
    value = dconfig->sensitivity * value; // Apply the sensitivity for this axis & direction
    rawValue = value;                     // Store the raw value for debugging purposes

    // Apply the modifier function for this axis & direction
    modifier(dconfig->modFuncType);

    // Apply any gate for this axis & direction.
    if (abs(value) < dconfig->gate) {
        value = 0;
    }

    // Invert the motion if necessary
    value = (config->inversion) ? -value : value; // Invert the value if necessary
    modifiedValue = value;                        // Store the modified value for debugging purposes
}

void Axis::modifier(ModFunc_t type) {
    value = constrain(value, -350, 350); // Constrain the value to the range of -350 to 350

    switch (type) {
    case mfLINEAR:
        value = constrain(value, -350, 350);
        break;
    case mfSQUARED:
        value = 350 * pow(value / 350.0, 2) * sign(value); // sign putting out -1 or 1 depending on sign of value. (Is needed because x^2 will always be positive)
        break;
    case mfTANGENT:
        value = 350 * tan(value / 350.0);
        break;
    case mfSQUARDED_TANGENT:
        value = 350 * tan(pow(value / 350.0, 2) * sign(value)); // sign putting out -1 or 1 depending on sign of value. (Is needed because x^2 will always be positive)
        break;
    case mfCUBED_TANGENT:
        value = 350 * tan(pow(value / 350.0, 3)); // sign putting out -1 or 1 depending on sign of value. (Is needed because x^3 will always be positive)
        break;
    default:
        break;
    }

    value = constrain(value, -350, 350);
}

void Axis::accept(IPrinterVisitor &visitor) {
    visitor.visit(*this);
}