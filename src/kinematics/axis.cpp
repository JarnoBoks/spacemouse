#include "axis.h"
#include <math.h>
#define sign(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0)) // Define Signum Function
#include "config.h"
#include "kinematics/axisconfig.h"
#include "serialoutput/idebugmonitor.h"
#include "hardware/hardware.h"
#include "led/lightbehavior.h"
#include "hardware/hardware_hall.h"
#include "hardware/hardware_joystick.h"

Axis::Axis() : type(UNINITIALIZED) {
    // Default constructor initializes the axis to UNINITIALIZED
    this->config = new AxisConfig(); // Create a new AxisConfig object for this axis

    this->hardware = HW_TYPE::getInstance(); // Initialize the hardware with Hall effect sensors

    this->light = nullptr; // Initialize light to nullptr
    this->value = 0;       // Initialize value to 0
}

Axis::Axis(AxisType_t type) : type(type) {
    // Initialize the axis with the given type
    this->config = new AxisConfig(type); // Create a new AxisConfig object for this axis

    this->hardware = HW_TYPE::getInstance(); // Initialize the hardware with Hall effect sensors

    this->light = nullptr; // Initialize light to nullptr
    this->value = 0;       // Initialize value to 0
}

int16_t Axis::getValue() const {
    return value;
}

void Axis::calculateValue() {
    value = hardware->calculateRawValue(type);

    DirectionConfig *dconfig = (value > 0) ? &this->config->posConfig : &this->config->negConfig; // Get the config for the current axis and direction

    // Apply the sensitivity for this axis & direction
    value = dconfig->sensitivity * value; // Apply the sensitivity for this axis & direction

    // Apply the modifier function for this axis, override the default one if necessary
    modifier(dconfig->modFuncType); // Apply the modifier function for this axis, override the default one if necessary

    // Apply any gate for this axis.
    if (abs(value) < dconfig->gate) {
        value = 0;
    }

    // Invert the motion if necessary
    value = (config->inversion) ? -value : value; // Invert the value if necessary
}

void Axis::setLedLight(LightBehavior *behavior) {
    light = behavior;
    if (light) {
        // light->setLight(value);
    }
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
