
#include "iobserver.h"

// FIXME - The update function should be pure virtual, but we need to check if this is possible with the current implementation of the observer pattern.
void IObserver::update(Kinematics *kinematics) {
    // Doing nothing here, as this is a virtual function to be overridden by derived classes.
    // Additional implementation can be added in derived classes.
}

void IObserver::update(Hardware *hardware) {
    // Doing nothing here, as this is a virtual function to be overridden by derived classes.
    // Additional implementation can be added in derived classes.
}
