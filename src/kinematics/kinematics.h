#pragma once

#include "common/Observable.hpp" // For IObservable interface & Base class

#define MAX_AXES 6

#include "axis/axes/axistype.h" // For AxisType_t
#include "axis/axes/Axis.hpp"   // For Axis class
#include "observers/IObserver.hpp"
#include "axis/AxisCollection.hpp"

/// @brief Number of observers that can be added to this object
/// @details This is a constant value that defines the maximum number of observers that can be added to the collection.
constexpr uint8_t c_MAX_KINEMATICS_OBSERVERS = 4; // Maximum number of observers for the axis collection

class KinematicsConfig;

// --- Kinematics Singleton ---
class Kinematics : public Observable {
private:
    static Kinematics *instance;
    AxisCollection *m_axisCollection = nullptr; // Pointer to the axis collection

    KinematicsConfig *config = nullptr;

    Kinematics();

    void _applyExclusiveMode();
    void _applySwitchYZ();

    void _applyKillSwitch(const AxisType_t start, const AxisType_t end, const bool killSwitchActive) {
        // Set strategy for the rotation axes to kill switch
        for (uint8_t i = start; i <= end; i++) {
            static_cast<Axis *>(m_axisCollection->getItem(i))->setKillSwitchActive(killSwitchActive); // Set the kill switch state for the axis
        }
    };

public:
    static Kinematics *getInstance();

    inline void setAxisCollection(AxisCollection *axisCollection) {
        m_axisCollection = axisCollection; // Set the axis collection
    };

    // TODO - REMOVE
    inline Axis *getAxis(const AxisType_t type) {
        return static_cast<Axis *>(m_axisCollection->getItem(type)); // Get the axis from the collection
    };
    // TODO - REMOVE
    inline Axis *getAxis(const char *name) {
        return static_cast<Axis *>(m_axisCollection->getItem(name)); // Get the axis from the collection
    };
    inline KinematicsConfig *getConfig() const { return config; } // Getter for config

    void processKinematics();
    const AxisType_t getMainAxis(Axis *axis); // Get the main and secondary axis for the kinematics

    // Functionality for the kill switches
    void killRotation(const bool killSwitchActive = true) {
        _applyKillSwitch(AxisType_t::ROTX, AxisType_t::ROTZ, killSwitchActive); // Set strategy for the rotation axes to kill switch
    };
    void killTranslation(const bool killSwitchActive = true) {
        _applyKillSwitch(AxisType_t::TRANSX, AxisType_t::TRANSZ, killSwitchActive);
    };
};
