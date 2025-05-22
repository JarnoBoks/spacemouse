#include "KinematicsAxis.hpp"

#include <common/esp_print.h> //REFACTOR - Move to cpp file

#include <knob/axis/KnobAxis.hpp>
#include <knob/KnobAxisCollection.hpp> // REFACTOR - Move to cpp file

void KinematicsAxis::evaluate() {
    if (m_knobAxis == nullptr) {
        ESP_WARN("KnobAxis not set");
        return; // Error: KnobAxis not found, exit the function
    }

    // Get the inversion setting from the knob Axis
    const bool invert = m_knobAxis->getConfig()->getInvert();
    int16_t value = m_knobAxis->getFinValue(); // Get the final value from the knob Axis
    m_finValue = (invert) ? -value : value;    // Invert the value if the inversion setting is enabled

    // Notify observers of changes in the Kinematics Axis
    Observable::notifyObservers();
}