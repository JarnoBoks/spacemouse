
#include "hardware.h"

void Hardware::setAnalogReference(const uint8_t voltage) {
    referenceVoltage = voltage;
#ifdef ARDUINO_ARCH_AVR
    analogReference(referenceVoltage);
#endif
}

/**
 * @brief Update the sensor values by reading and applying sensor calibration.
 */
void Hardware::updateSensorValues() {
    // Trigger all sensors to read their values (raw & centered) and apply their calibration values (map min/max/, invert & deadzone)
    for (uint8_t i = 0; i < MAX_SENSORS; i++) {
        if (sensors[i]) {
            sensors[i]->readValue();        // Read Analog value from the sensor & center it
            sensors[i]->applyCalibration(); // Apply calibration to the sensor
        }
    }
}

void Hardware::attachObserver(IDebugMonitor *observer) {
    if (observerCount < MAX_HARDWARE_OBSERVERS) {
        Serial.println(F("Hardware::attachObserver: "));
        // insert the observer into the array, at position observerCount and increase the count after inserting.
        observers[observerCount++] = observer;
    } else {
        // TODO - Handle the case when the observer array is full. Maybe remove the oldest observer or ignore the new one?
    }
};

void Hardware::detachObserver(IDebugMonitor *observer) {
    Serial.println(F("Hardware::detachObserver: "));
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
