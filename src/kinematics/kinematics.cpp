#include "Kinematics.hpp"
#include "config.h"
#include <kinematics/config/kinematicsconfig.hpp>

#include <knob/KnobAxisCollection.hpp>
// #include <knob/Axis/KnobAxis.hpp>

#include <kinematics/visitor/ExclusiveMovementVisitor.hpp>

Kinematics::Kinematics()
    : Observable(c_MAX_KINEMATICS_OBSERVERS),
      m_knobAxisCollection(nullptr),
      m_config(new KinematicsConfig()) {}

Kinematics::Kinematics(KnobAxisCollection *knobAxisCollection)
    : Observable(c_MAX_KINEMATICS_OBSERVERS),
      m_knobAxisCollection(knobAxisCollection),
      m_config(new KinematicsConfig()) {} // Constructor with knob axis collection

Kinematics::~Kinematics() {
    delete m_config; // Delete the kinematics configuration
}

void Kinematics::_applyExclusiveMode() {
    if (!m_config || !m_config->getExclusiveMode()) {
        return;
    }

    // Create a visitor for the exclusive movement
    ExclusiveMovementVisitor EMvisitor;
    m_knobAxisCollection->accept(EMvisitor); // Accept the visitor to apply the exclusive movement
}

void Kinematics::_applySwitchYZ() {
    if (!m_config || !m_config->getSwitchYZ()) {
        return;
    }

    static_cast<KnobAxis *>(m_knobAxisCollection->getItem(MotionVector_t::TRANSY))->setType(MotionVector_t::TRANSZ);
    static_cast<KnobAxis *>(m_knobAxisCollection->getItem(MotionVector_t::TRANSZ))->setType(MotionVector_t::TRANSY);
    static_cast<KnobAxis *>(m_knobAxisCollection->getItem(MotionVector_t::ROTY))->setType(MotionVector_t::ROTZ);
    static_cast<KnobAxis *>(m_knobAxisCollection->getItem(MotionVector_t::ROTZ))->setType(MotionVector_t::ROTY);
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
const MotionVector_t Kinematics::getMainAxis(KnobAxis *motionVector) {
    MotionVector_t idMainAxis = MotionVector_t::MV_UNINITIALIZED;
    int16_t maximumVelocity = 0;

    // Loop through all axes to find the one with the biggest velocity
    for (int i = 0; i < MotionVector_t::MV_LENGTH; i++) {
        // TODO int16_t absvalue = abs(m_knobMotionVectors->getAxis(i)->getFinValue()); // Get the value of the motionVector

        // Is the value of this motionVector greater than deadzone and greater than any of the motionVector before?
        /* TODO if ((absvalue > maximumVelocity) && (absvalue > VELOCITYDEADZONEFORLED)) {
            maximumVelocity = absvalue;
            idMainAxis = static_cast<MotionVector_t>(i);
        } */
    }
    if (idMainAxis == MotionVector_t::MV_UNINITIALIZED) {
        motionVector = nullptr; // Set the motionVector to nullptr if no motionVector is found
    } else {
        // TODO motionVector = static_cast<KnobMotionVector *>(m_knobMotionVectors->getItem(idMainAxis));
        //  REVIEW - Check if the pointer assignment is correct. It should be a reference to the motionVector, not a pointer.
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
