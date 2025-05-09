#include "kinematics.h"
#include "hardware/hardware.h"
#include "config.h"
#include "KinematicsConfig.h"
#include "KinematicsConfig.h"

// Static pointer for the Singleton instance of Kinematics
// This pointer is initialized to nullptr, indicating that the instance has not been created yet.
Kinematics *Kinematics::instance = nullptr;

/**
 * @brief  Retrieves the singleton instance of the Kinematics class.
 * @return A reference to the Kinematics instance.
 */
Kinematics *Kinematics::getInstance() {
    if (!instance) {
        instance = new Kinematics();
    }
    return instance;
}

/**
 * @brief  Constructor for the Kinematics class.
 * @details Initializes a Kinematics object by initializing its config and initializing the
 *          axes with their respective configurations and hardware.
 * @note   This constructor sets up all the axes for the Spacemouse functionality.
 */
Kinematics::Kinematics() : Observable(c_MAX_KINEMATICS_OBSERVERS),
                           m_axisCollection(new AxisCollection()),
                           config(new KinematicsConfig()) {
    m_axisCollection->setup(); // Set up the axis collection based on the configuration
}

/**
 * @brief Processes the kinematics for all axes and updates their values. Notifies attached observers of these changes.
 * @details This function calculates the values for each axis based on the hardware input and configuration.
 */
void Kinematics::processKinematics() {
    Hardware *hardware = Hardware::getInstance();
    hardware->evaluateSensorCollection(); // Update the sensor values from the hardware
    for (int i = 0; i < AxisType_t::LENGTH; i++) {
        int16_t raw = hardware->calculateRawValue(static_cast<AxisType_t>(i));  // Get the raw value from the hardware
        static_cast<Axis *>(m_axisCollection->getItem(i))->calculateValue(raw); // Calculate the value for each axis
    }
    hardware->notifyObservers(); // Notify observers of changes in the hardware
    notifyObservers();           // Notify observers of changes in the kinematics
}

// REVIEW - This should be a decorator function for the axis class, but we need to check if we can use the same function for both classes.
// Define a macro to simplify the access to the sensor values
#define ABSVAL(x) abs(static_cast<Axis *>(m_axisCollection->getItem(x))->getValue())
void Kinematics::_applyExclusiveMode() {
    if (config != nullptr && config->exclusiveMode) {
        uint16_t totalRot = ABSVAL(ROTX) + ABSVAL(ROTY) + ABSVAL(ROTZ);         // Total rotation value
        uint16_t totalTrans = ABSVAL(TRANSX) + ABSVAL(TRANSY) + ABSVAL(TRANSZ); // Total translation value

        // If the total rotation is greater than the total translation, set translation axes to 0
        // Otherwise, set rotation axes to 0
        int8_t startAxis = -1;
        int8_t endAxis = -1;
        if (totalRot > totalTrans) {
            startAxis = TRANSX;
            endAxis = TRANSZ; // Set translation axes to 0
        } else {
            startAxis = ROTX;
            endAxis = ROTZ; // Set translation axes to 0
        }

        for (int i = startAxis; i <= endAxis; i++) {
            static_cast<Axis *>(m_axisCollection->getItem(i))->setValue(0); // Set translation axes to 0
        }
    }
}
#undef ABSVAL

// REVIEW - What is the order of the exclusive mode and switch YZ?
// REVIEW - Can we switch the entire axis at once in the array?
#define ATRANSY static_cast<Axis *>(m_axisCollection->getItem(TRANSY))
#define ATRANSZ static_cast<Axis *>(m_axisCollection->getItem(TRANSZ))
#define AROTY static_cast<Axis *>(m_axisCollection->getItem(ROTY))
#define AROTZ static_cast<Axis *>(m_axisCollection->getItem(ROTZ))

void Kinematics::_applySwitchYZ() {
    if (config != nullptr && config->switchYZ) {
        int16_t tmp = 0;
        tmp = ATRANSY->getValue();

        ATRANSY->setValue(ATRANSZ->getValue());
        ATRANSZ->setValue(tmp);

        tmp = AROTY->getValue();
        AROTY->setValue(AROTZ->getValue());
        AROTZ->setValue(tmp);
    }
}
#undef ATRANSY
#undef ATRANSZ
#undef AROTY
#undef AROTZ

/**
 * @brief Get the axis with the largest velocity.
 * @param axis Pointer to the Axis object to be set with the main axis.
 * @return The AxisId for the axis with the largest velocity. Returns enumAxis_t::UNITIALIZED if all axes are in the VELOCITYDEADZONEFORLED
 * @see Ledring.cpp for usage
 */
#ifndef VELOCITYDEADZONEFORLED    // Defined in config.h
#define VELOCITYDEADZONEFORLED 10 // Deadzone for the LED ring, if the velocity is below this value, it will not be displayed on the LED ring
#endif

// REFACTOR - Shoud return a pointer to the axis instead of the AxisType_t enum. This will make it easier to use in the LED ring and other classes.
const AxisType_t Kinematics::getMainAxis(Axis *axis) {
    AxisType_t idMainAxis = AxisType_t::UNINITIALIZED;
    int16_t maximumVelocity = 0;

    // Loop through all axes to find the one with the biggest velocity
    for (int i = 0; i < AxisType_t::LENGTH; i++) {
        int16_t absvalue = abs(static_cast<Axis *>(m_axisCollection->getItem(i))->getValue()); // Get the value of the axis

        // Is the value of this axis greater than deadzone and greater than any of the axis before?
        if ((absvalue > maximumVelocity) && (absvalue > VELOCITYDEADZONEFORLED)) {
            maximumVelocity = absvalue;
            idMainAxis = static_cast<AxisType_t>(i);
        }
    }
    if (idMainAxis == AxisType_t::UNINITIALIZED) {
        axis = nullptr; // Set the axis to nullptr if no axis is found
    } else {
        axis = static_cast<Axis *>(m_axisCollection->getItem(idMainAxis));
        // REVIEW - Check if the pointer assignment is correct. It should be a reference to the axis, not a pointer.
    }
    return idMainAxis;
}

#if 0  // REMOVE - Keeping for PGM string example at the moment
Axis *Kinematics::getAxis(const char *name) {
    // TODO - Make progmem string for the axis names
    const __FlashStringHelper *axisNames[] PROGMEM = {F("TX"), F("TY"), F("TZ"), F("RX"), F("RY"), F("RZ")}; // Axis names
    for (int i = 0; i < 6; i++) {
        if (strcmp(name, (const char *)pgm_read_word(&(axisNames[i]))) == 0) {
            return axes[i]; // Return the corresponding axis
        }
    }
    return nullptr; // Axis not found, return nullptr
}
#endif // REMOVE
