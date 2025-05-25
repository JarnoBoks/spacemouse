#pragma once

#include "common/Observable.hpp"        // For IObservable interface & Base class
#include <knob/KnobAxisCollection.hpp>  // For KnobAxisCollection
#include <base/axis/MotionVectorType.h> // For MotionVector_t enum
#include <observers/IObserver.hpp>

/// @brief Number of observers that can be added to this object
/// @details This is a constant value that defines the maximum number of observers that can be added to the collection.
constexpr uint8_t c_MAX_KINEMATICS_OBSERVERS = 4; // Maximum number of observers for the kinematics collection

class KinematicsConfig;
class KinematicsAxisCollection;
class KnobAxisCollection;

class Kinematics : public Observable {
private:
    KnobAxisCollection *m_knobAxisCollection = nullptr;
    KinematicsConfig *m_config = nullptr; // Pointer to the kinematics configuration

    void _applyExclusiveMode();
    void _applySwitchYZ();

#if 0 // REMOVE
    void _applyKillSwitch(const AxisType_t start, const AxisType_t end, const bool killSwitchActive) {
        // Set strategy for the rotation axes to kill switch
        for (uint8_t i = start; i <= end; i++) {
            static_cast<Axis *>(m_knobMotionVectors->getItem(i))->setKillSwitchActive(killSwitchActive); // Set the kill switch state for the axis
        }
    };
#endif

public:
    Kinematics() = delete; // Default constructor is deleted to prevent instantiation without parameters
    Kinematics(KnobAxisCollection *knobAxisCollection);

    ~Kinematics();

    void evaluate() {
        _applyExclusiveMode(); // Apply exclusive mode if enabled
        _applySwitchYZ();      // Apply switch YZ if enabled

        Observable::notifyObservers(); // Notify observers of changes in the kinematics
    }

    KinematicsConfig *getConfig() const { return m_config; } // Getter for config
    const MotionVector_t getMainAxis(KnobAxis *axis);        // Get the main and secondary axis for the kinematics
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
