#pragma once

#include "common/Observable.hpp" // For IObservable interface & Base class

#define MAX_AXES 6

#include <axis/MotionVectorType.h> // For MotionVector_t enum
#include <axis/axes/Axis.hpp>      // For Axis class
#include <observers/IObserver.hpp>
#include <axis/KnobMotionVectorCollection.hpp>

/// @brief Number of observers that can be added to this object
/// @details This is a constant value that defines the maximum number of observers that can be added to the collection.
constexpr uint8_t c_MAX_KINEMATICS_OBSERVERS = 4; // Maximum number of observers for the kinematics collection

class KinematicsConfig;

// --- Kinematics Singleton ---
class Kinematics : public Observable {
private:
    static Kinematics *instance;
    KnobMotionVectorCollection *m_knobMotionVectors = nullptr; // Pointer to the axis collection

    KinematicsConfig *config = nullptr;

    Kinematics();

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
    static Kinematics *getInstance();

    inline void setAxisCollection(KnobMotionVectorCollection *axisCollection) {
        m_knobMotionVectors = axisCollection; // Set the axis collection
    };

    inline KinematicsConfig *getConfig() const { return config; } // Getter for config

    void execute() {
        _applyExclusiveMode();         // Apply exclusive mode if enabled
        _applySwitchYZ();              // Apply switch YZ if enabled
        Observable::notifyObservers(); // Notify observers of changes in the kinematics
    }

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
