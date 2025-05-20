#pragma once

#include "common/Observable.hpp" // For IObservable interface & Base class

#include <motionvector/MotionVectorType.h> // For MotionVector_t enum
#include <observers/IObserver.hpp>

/// @brief Number of observers that can be added to this object
/// @details This is a constant value that defines the maximum number of observers that can be added to the collection.
constexpr uint8_t c_MAX_KINEMATICS_OBSERVERS = 4; // Maximum number of observers for the kinematics collection

class KinematicsConfig;
class KinematicsMotionVectorCollection;
class KnobMotionVector; // REMOVE

class Kinematics : public Observable {
private:
    static Kinematics *instance;

    KinematicsMotionVectorCollection *m_transMotionVectors = nullptr; // Pointer to the translational kinematic MotionVectors collection
    KinematicsMotionVectorCollection *m_rotMotionVectors = nullptr;   // Pointer to the rotational kinematic MotionVectors collection

    KinematicsConfig *m_config = nullptr; // Pointer to the kinematics configuration

    void _applyExclusiveMode();
    void _applySwitchYZ();

    void _createVector(const KnobMotionVectorCollection *knobVectors,
                       const MotionVector_t type,
                       IObserver *observer);

#if 0 // REMOVE
    void _applyKillSwitch(const AxisType_t start, const AxisType_t end, const bool killSwitchActive) {
        // Set strategy for the rotation axes to kill switch
        for (uint8_t i = start; i <= end; i++) {
            static_cast<Axis *>(m_knobMotionVectors->getItem(i))->setKillSwitchActive(killSwitchActive); // Set the kill switch state for the axis
        }
    };
#endif

public:
    Kinematics();
    ~Kinematics();

    void setup(const KnobMotionVectorCollection *knobMotionVectors,
               IObserver *hidEventBufferTranslation,
               IObserver *hidEventBufferRotation);

    void evaluate() {
        _applyExclusiveMode(); // Apply exclusive mode if enabled
        _applySwitchYZ();      // Apply switch YZ if enabled

        Observable::notifyObservers(); // Notify observers of changes in the kinematics
    }
    KinematicsConfig *getConfig() const { return m_config; }  // Getter for config
    const MotionVector_t getMainAxis(KnobMotionVector *axis); // Get the main and secondary axis for the kinematics
#if 0
    // Functionality for the kill switches
    void killRotation(const bool killSwitchActive = true) {
        _applyKillSwitch(AxisType_t::ROTX, AxisType_t::ROTZ, killSwitchActive); // Set strategy for the rotation axes to kill switch
    };

    void killTranslation(const bool killSwitchActive = true) {
        _applyKillSwitch(AxisType_t::TRANSX, AxisType_t::TRANSZ, killSwitchActive);
    };
#endif
};
