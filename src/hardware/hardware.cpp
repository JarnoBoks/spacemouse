#include "hardware.h"

// Only log to serial if not using Arduino AVR architecture
#ifndef ARDUINO_ARCH_AVR
#ifndef ESP_PRINT(x)
#define ESP_PRINT(x) Serial.println(x)
#define ESP_DBG(x) Serial.println(x)
#endif
#else
#define ESP_PRINT(x)
#define ESP_DBG(x)
#endif

// -- Static part of the class (= application specific)
// In this part the hardware class is defined and the static instance is created

Hardware *Hardware::_instance = nullptr;

Hardware *Hardware::getInstance() {
    /*if (_instance == nullptr) {
        _instance = HW_TYPE::getInstance(); // Create the hardware instance based on the defined hardware type
    }*/
    return _instance;
}

void Hardware::setAnalogReference(const bool isDebug) {
    referenceVoltage = DEFAULT; // Set the default reference voltage to DEFAULT
#ifdef ARDUINO_ARCH_AVR
    analogReference(referenceVoltage); // Set the analog reference voltage to DEFAULT
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

Sensor *Hardware::getSensorByName(const char *name) const {
    for (int i = 0; i < MAX_SENSORS; i++) {
        if (sensors[i] != nullptr && sensors[i]->isCurrentSensor(name)) {
            return sensors[i];
        }
    }
    return nullptr; // Return nullptr if no matching sensor is found
};

/**
 * @brief Attach an observer to the hardware class.
 * @param observer Pointer to the observer to be attached.
 * @details This function adds the observer to the observers array and increases the observer count.
 *          If the array is full, it does not add the new observer and can be modified to handle this case.
 */
void Hardware::attachObserver(IObserver *observer) {
    if (observerCount < MAX_HARDWARE_OBSERVERS) {
        ESP_DBG(F("Hardware::attachObserver: "));
        // insert the observer into the array, at position observerCount and increase the count after inserting.
        observers[observerCount++] = observer;
    } else {
        // TODO - Handle the case when the observer array is full. Maybe remove the oldest observer or ignore the new one?
    }
};

void Hardware::detachObserver(IObserver *observer) {
    ESP_DBG(F("Hardware::detachObserver: "));
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
