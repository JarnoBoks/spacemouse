#pragma once

#define MAX_AXES 6

#include "..\axis\axes\axistype.h" // For AxisType_t
#include "..\axis\axes\Axis.hpp"   // For Axis class
#include "observers/IObserver.hpp"
#include "axis/AxisCollection.hpp"

class KinematicsConfig;

// --- Kinematics Singleton ---
class Kinematics {
private:
    static Kinematics *instance;
    AxisCollection *m_axisCollection = nullptr; // Pointer to the axis collection

    KinematicsConfig *config = nullptr;

    IObserver *observers[MAX_KINEMATICS_OBSERVERS] = {nullptr}; // Array of observers
    uint8_t observerCount = 0;

    Kinematics();

    void _applyExclusiveMode();
    void _applySwitchYZ();

    void _applyKillSwitch(const AxisType_t start, const AxisType_t end, const bool killSwitchActive) {
        // Set strategy for the rotation axes to kill switch
        for (uint8_t i = start; i <= end; i++) {
            m_axisCollection->getAxis(i)->setKillSwitchActive(killSwitchActive); // Set the kill switch state for the axis
        }
    };

public:
    static Kinematics *getInstance();

    inline Axis *getAxis(const AxisType_t type) {
        return m_axisCollection->getAxis(type); // Get the axis from the collection
    };
    inline Axis *getAxis(const char *name) {
        return m_axisCollection->getAxis(name); // Get the axis from the collection by name
    };

    inline KinematicsConfig *getConfig() const { return config; } // Getter for config

    void attachObserver(IObserver *observer);
    void detachObserver(IObserver *observer);
    void notifyObservers(); // Notify all observers of changes

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
