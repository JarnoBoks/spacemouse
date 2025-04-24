

#include "axis.h"
#include "config.h"

Axis::Axis(AxisType_t type) : type(type) {
    // Initialize the axis with the given type
    this->config = new AxisConfig(type); // Create a new AxisConfig object for this axis

#ifdef HALLEFFECT
    this->hardware = HALL::getInstance(); // Initialize the hardware with Hall effect sensors
#elif defined(JOYSTICK)
    this->hardware = JOYSTICK::getInstance(); // Initialize the hardware with Joystick
#endif

    this->light = nullptr; // Initialize light to nullptr
    this->value = 0;       // Initialize value to 0
}

void Axis::attachObserver(DebugMonitor *observer) {
    if (observerCount < MAX_AXIS_OBSERVERS) {
        observers[observerCount++] = observer;
    }
}

void Axis::detachObserver(DebugMonitor *observer) {
    for (uint8_t i = 0; i < observerCount; i++) {
        if (observers[i] == observer) {
            observers[i] = observers[--observerCount];
            break;
        }
    }
}
void Axis::notifyObservers() {
    for (uint8_t i = 0; i < observerCount; i++) {
        observers[i]->update(this);
    }
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

    value = dconfig->modifier->modify(value); // Apply the modifier function for this axis, override the default one if necessary

    // Apply any gate for this axis.
    if (abs(value) < dconfig->gate) {
        value = 0;
    }

    // Invert the motion if necessary
    value = (config->inversion) ? -value : value; // Invert the value if necessary

    notifyObservers();
}

void Axis::setLedLight(LightBehavior *behavior) {
    light = behavior;
    if (light) {
        // light->setLight(value);
    }
}
