#include "Kinematics.hpp"
#include "config.h"
#include <kinematics/config/kinematicsconfig.hpp>
#include <kinematics/axiscollection/KinematicsAxisCollection.hpp>
#include <kinematics/axis/KinematicsAxis.hpp>
#if 0
//NOTE See AxisBase.hpp for the reason why we don't use this
#include <kinematics/axis/deprecated/KinematicsAxisRotation.hpp>
#include <kinematics/axis/deprecated/KinematicsAxisTranslation.hpp>
#endif

#include <knob/KnobAxisCollection.hpp>
#include <knob/Axis/KnobAxis.hpp>

Kinematics::Kinematics()
    : Observable(c_MAX_KINEMATICS_OBSERVERS),
      m_transMotionVectors(new KinematicsAxisCollection()),
      m_rotMotionVectors(new KinematicsAxisCollection()),
      m_config(new KinematicsConfig()) {}

Kinematics::~Kinematics() {
    delete m_transMotionVectors; // Delete the translational kinematic MotionVectors collection
    delete m_rotMotionVectors;   // Delete the rotational kinematic MotionVectors collection
    delete m_config;             // Delete the kinematics configuration
}

void Kinematics::_createAxis(const KnobAxisCollection *knobVectors,
                             const MotionVector_t type,
                             IObserver *hidEventbuffer) {

    KnobAxis *knobAxis = knobVectors->getAxis(type);
    if (knobAxis == nullptr) {
        return; // Error: KnobAxis not found, exit the function
    }

    KinematicsAxis *kinAxis = nullptr;
    if (knobAxis->isTranslation()) {
        kinAxis = new KinematicsAxis(type, knobAxis);
        m_transMotionVectors->add(kinAxis);
    } else {
        kinAxis = new KinematicsAxis(type, knobAxis);
        m_rotMotionVectors->add(kinAxis);
    }
    kinAxis->attachObserver(hidEventbuffer);
}

void Kinematics::setup(const KnobAxisCollection *knobAxisCollection,
                       IObserver *hidEventBufferTranslation,
                       IObserver *hidEventBufferRotation) {

    _createAxis(knobAxisCollection, MotionVector_t::TRANSX, hidEventBufferTranslation);
    _createAxis(knobAxisCollection, MotionVector_t::TRANSY, hidEventBufferTranslation);
    _createAxis(knobAxisCollection, MotionVector_t::TRANSZ, hidEventBufferTranslation);
    _createAxis(knobAxisCollection, MotionVector_t::ROTX, hidEventBufferRotation);
    _createAxis(knobAxisCollection, MotionVector_t::ROTY, hidEventBufferRotation);
    _createAxis(knobAxisCollection, MotionVector_t::ROTZ, hidEventBufferRotation);
}

void Kinematics::_applyExclusiveMode() {
    if (!m_config || !m_config->getExclusiveMode()) {
        return;
    }

    // Create a visitor for the exclusive movement
    // ExclusiveMovementVisitor EMvisitor;
    // m_knobMotionVectors->accept(EMvisitor); // Accept the visitor to apply the exclusive movement
    /* REFACTOR - For now the collection passes the Zeroing to the motionVector, but this isn't SOLID
    design and we cannot store the intermediate value for debugging. Furthermore, we should make this
    function an observer.
    */
    if (m_transMotionVectors->getTotalVelocity() > m_rotMotionVectors->getTotalVelocity()) {
        // If the total translation is greater than the total rotation, set rotation axes to 0
        m_rotMotionVectors->setAllToZero();
    } else {
        m_transMotionVectors->setAllToZero(); // Set translation axes to 0
    }
}

void Kinematics::_applySwitchYZ() {
    if (!m_config || !m_config->getSwitchYZ()) {
        return;
    }

    static_cast<KinematicsAxis *>(m_transMotionVectors->getItem(MotionVector_t::TRANSY))->setType(MotionVector_t::TRANSZ);
    static_cast<KinematicsAxis *>(m_transMotionVectors->getItem(MotionVector_t::TRANSZ))->setType(MotionVector_t::TRANSY);
    static_cast<KinematicsAxis *>(m_rotMotionVectors->getItem(MotionVector_t::ROTY))->setType(MotionVector_t::ROTZ);
    static_cast<KinematicsAxis *>(m_rotMotionVectors->getItem(MotionVector_t::ROTZ))->setType(MotionVector_t::ROTY);
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
