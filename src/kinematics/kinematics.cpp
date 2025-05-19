#include "kinematics.h"
#include "config.h"
#include "kinematics/config/kinematicsconfig.hpp"
#include <axis/KnobMotionVectorCollection.hpp>
#include <axis/axes/KnobMotionVector.hpp>

#include <visitors/ExclusiveMovementVisitor.hpp>
#include <visitors/SwitchYZVisitor.hpp>

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
                           config(new KinematicsConfig()) {
}

#if 0 // REMOVE
// Define a macro to simplify the access to the sensor values
#define ABSVAL(x) abs(static_cast<Axis *>(m_axisCollection->getItem(x))->getFinValue())
#endif
void Kinematics::_applyExclusiveMode() {
    if (config && config->getExclusiveMode()) {
        // Create a visitor for the exclusive movement
        ExclusiveMovementVisitor EMvisitor;
        m_knobMotionVectors->accept(EMvisitor); // Accept the visitor to apply the exclusive movement
    }
#if 0 // REMOVE - After testing
    if (config && config->getExclusiveMode()) {
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
            static_cast<Axis *>(m_knobMotionVectors->getItem(i))->setFinValue(0); // Set translation axes to 0
        }
    }
#endif
}
#if 0 // REMOVE
#undef ABSVAL
#endif

#if 0 // REMOVE - After testing
// REVIEW - What is the order of the exclusive mode and switch YZ?
// REVIEW - Can we switch the entire motionVector at once in the array?
#define ATRANSY m_axisCollection->getAxis(TRANSY)
#define ATRANSZ m_axisCollection->getAxis(TRANSZ)
#define AROTY m_axisCollection->getAxis(ROTY)
#define AROTZ m_axisCollection->getAxis(ROTZ)

void Kinematics::_applySwitchYZ() {
    if (config != nullptr && config->getSwitchYZ()) {
        int16_t tmp = 0;
        tmp = ATRANSY->getFinValue();

        ATRANSY->setFinValue(ATRANSZ->getFinValue());
        ATRANSZ->setFinValue(tmp);

        tmp = AROTY->getFinValue();
        AROTY->setFinValue(AROTZ->getFinValue());
        AROTZ->setFinValue(tmp);
    }
}
#undef ATRANSY
#undef ATRANSZ
#undef AROTY
#undef AROTZ
#endif

void Kinematics::_applySwitchYZ() {
    if (config != nullptr && config->getSwitchYZ()) {
        // Create a visitor for the switch YZ
        SwitchYZVisitor YZvisitor;
        m_knobMotionVectors->accept(YZvisitor); // Accept the visitor to apply the switch YZ
    }
}

/**
 * @brief Get the KnobMotionVector with the largest velocity.
 * @param motionVector Pointer to the KnobMotionVector object to be set with to the object with the largest velocity.
 * @return The id for the KnobMotionVector with the largest velocity.
 * @retval MotionVector_t::UNITIALIZED if all axes are in the VELOCITYDEADZONEFORLED
 * @see Ledring.cpp for usage
 */
#ifndef VELOCITYDEADZONEFORLED    // Defined in config.h
#define VELOCITYDEADZONEFORLED 10 // Deadzone for the LED ring, if the velocity is below this value, it will not be displayed on the LED ring
#endif

// REFACTOR - Shoud return a pointer to the motionVector instead of the MotionVector_t enum. This will make it easier to use in the LED ring and other classes.
const MotionVector_t Kinematics::getMainAxis(KnobMotionVector *motionVector) {
    MotionVector_t idMainAxis = MotionVector_t::UNINITIALIZED;
    int16_t maximumVelocity = 0;

    // Loop through all axes to find the one with the biggest velocity
    for (int i = 0; i < MotionVector_t::LENGTH; i++) {
        int16_t absvalue = abs(m_knobMotionVectors->getMotionVector(i)->getFinValue()); // Get the value of the motionVector

        // Is the value of this motionVector greater than deadzone and greater than any of the motionVector before?
        if ((absvalue > maximumVelocity) && (absvalue > VELOCITYDEADZONEFORLED)) {
            maximumVelocity = absvalue;
            idMainAxis = static_cast<MotionVector_t>(i);
        }
    }
    if (idMainAxis == MotionVector_t::UNINITIALIZED) {
        motionVector = nullptr; // Set the motionVector to nullptr if no motionVector is found
    } else {
        motionVector = static_cast<KnobMotionVector *>(m_knobMotionVectors->getItem(idMainAxis));
        // REVIEW - Check if the pointer assignment is correct. It should be a reference to the motionVector, not a pointer.
    }
    return idMainAxis;
}

#if 0 // REMOVE - Keeping for PGM string example at the moment
Axis *Kinematics::getAxis(const char *name) {
    // TODO - Make progmem string for the motionVector names
    const __FlashStringHelper *axisNames[] PROGMEM = {F("TX"), F("TY"), F("TZ"), F("RX"), F("RY"), F("RZ")}; // Axis names
    for (int i = 0; i < 6; i++) {
        if (strcmp(name, (const char *)pgm_read_word(&(axisNames[i]))) == 0) {
            return axes[i]; // Return the corresponding motionVector
        }
    }
    return nullptr; // Axis not found, return nullptr
}
#endif
