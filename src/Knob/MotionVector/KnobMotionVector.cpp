#include "KnobMotionVector.hpp"
#include "sensorscalculator/ISensorsCalculator.hpp" // Include the header file for ISensorsCalculator interface
#include <Knob/ModifierFunctionType.h>              // Include the header file for ModFunc_t enum

#include <math.h>
#define sign(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0)) // Define Signum Function

/**
 * @brief   Evaluates the KnobMotionVector by processing the sensor data and applying configurations.
 * @details This function retrieves the raw value from the sensor calculator, applies sensitivity, modifier function, and gate settings,
 *          and inverts the value if necessary. It also handles the kill switch functionality.
 * @note    The function uses the m_sensorsCalculator to get the raw value from the sensor.
 *          The final value is stored in m_finValue, which is the processed value after applying all configurations.
 */
void KnobMotionVector::evaluate() {
    m_sensorsCalculator->evaluate(this); // Get the raw value from the sensor calculator

    KnobVectorDirectionConfig *dconfig = (m_rawValue > 0) ? &this->m_Config->posConfig : &this->m_Config->negConfig; // Get the config for the current axis and direction

    // Apply the sensitivity for this MotionVector & knob direction (ie. positive or negative movement in the vector)
    m_snsValue = dconfig->getSensitivity() * m_rawValue; // Apply the sensitivity for this axis & direction

    // Apply the modifier function for this MotionVector & knob direction
    modifier(dconfig->getModFuncType());

    // Apply any gate for this axis & direction.
    m_finValue = (abs(m_modValue) < dconfig->getGate()) ? 0 : m_modValue; // Apply the gate for this axis & direction

    // Invert the motion if necessary
    m_finValue = (m_Config->inversion) ? -m_finValue : m_finValue; // Invert the value if necessary // REFACTOR - Move to kinematics
}

/**
 * @brief Applies a modifier function to the MotionVector.
 * @param type The type of modifier function to apply.
 * @details This function modifies the MotionVector value based on the specified modifier function type.
 *          The available modifier functions are linear, squared, tangent, squared tangent, and cubed tangent.
 *          The value is constrained to the range of -350 to 350.
 */
void KnobMotionVector::modifier(ModFunc_t type) {
    m_modValue = constrain(m_snsValue, -350, 350); // Constrain the value to the range of -350 to 350

    switch (type) {
    case mfLINEAR:
        m_modValue = constrain(m_modValue, -350, 350);
        break;
    case mfSQUARED:
        m_modValue = 350 * pow(m_modValue / 350.0, 2) * sign(m_modValue); // sign putting out -1 or 1 depending on sign of value. (Is needed because x^2 will always be positive)
        break;
    case mfTANGENT:
        m_modValue = 350 * tan(m_modValue / 350.0);
        break;
    case mfSQUARDED_TANGENT:
        m_modValue = 350 * tan(pow(m_modValue / 350.0, 2) * sign(m_modValue)); // sign putting out -1 or 1 depending on sign of value. (Is needed because x^2 will always be positive)
        break;
    case mfCUBED_TANGENT:
        m_modValue = 350 * tan(pow(m_modValue / 350.0, 3)); // sign putting out -1 or 1 depending on sign of value. (Is needed because x^3 will always be positive)
        break;
    default:
        break;
    }

    m_modValue = constrain(m_modValue, -350, 350);
}