#pragma once

#define MAX_AXES 6

#include "axis/axis.h"
#include "observers/IObserver.h"

class KinematicsConfig;

// --- Kinematics Singleton ---
class Kinematics {
private:
    static Kinematics *instance;
    Axis axes[AxisType_t::LENGTH];

    KinematicsConfig *config = nullptr;

    IObserver *observers[MAX_KINEMATICS_OBSERVERS] = {nullptr}; // Array of observers
    uint8_t observerCount = 0;

    Kinematics();

    void _applyExclusiveMode();
    void _applySwitchYZ();

public:
    static Kinematics *getInstance();

    Axis *getAxis(const AxisType_t type);
    Axis *getAxis(const char *name);

    void attachObserver(IObserver *observer);
    void detachObserver(IObserver *observer);
    void notifyObservers(); // Notify all observers of changes

    void processKinematics();
    const AxisType_t getMainAxis(Axis *axis); // Get the main and secondary axis for the kinematics
};
