#include "Kinematics.hpp"
#include "config.h"
#include <kinematics/config/kinematicsconfig.hpp>

#include <knob/KnobAxisCollection.hpp>
#include <kinematics/visitor/ExclusiveMovementVisitor.hpp>

#if 0
Kinematics::Kinematics()
    : Observable(c_MAX_KINEMATICS_OBSERVERS),
      m_knobAxisCollection(nullptr),
      m_config(nullptr) {}
#endif

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
