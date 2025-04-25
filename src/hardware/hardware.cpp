
#include "hardware.h"

void Hardware::setAnalogReference(const uint8_t voltage) {
    // analogReference(voltage);
    referenceVoltage = voltage;
    analogReference(referenceVoltage);
}

void Hardware::attachObserver(IDebugMonitor *observer) {
    if (observerCount < MAX_HARDWARE_OBSERVERS) {
        // insert the observer into the array, at position observerCount and increase the count after inserting.
        observers[observerCount++] = observer;
    } else {
        // TODO - Handle the case when the observer array is full. Maybe remove the oldest observer or ignore the new one?
    }
};

void Hardware::detachObserver(IDebugMonitor *observer) {
    // remove the observer from the array by replacing it with the last observer in the array and decrease the count.
    for (int i = 0; i < observerCount; i++) {
        if (observers[i] == observer) {
            observers[i] = observers[--observerCount];
            observers[observerCount] = nullptr;
            break;
        }
    }
};

void Hardware::notifyObservers() {
    for (int i = 0; i < observerCount; i++) {
        observers[i]->update(this); // Notify each observer
    }
};
