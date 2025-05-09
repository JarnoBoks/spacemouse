#pragma once

#include "config.h" // Allowed here, while this is a Collection class.

#include "common/ICollection.hpp"  // Include the ICollection interface header file
#include "observers/IObserver.hpp" // Include the IObserver interface header file

#include "sensor/sensors/Sensor.hpp" // Include the Key class header file
#include <stdint.h>

/// @brief Number of sensors that can be added to the collection.
/// @details This is a constant value that defines the maximum number of sensors that can be added to the collection.
/// @note As long as the specific hardware options for the SpaceMouse all have the same number of sensors, this is a good solution.
constexpr uint8_t cHW_MAX_SENSORS = 8;

/// @brief Number of observers that can be added to this collection.
/// @details This is a constant value that defines the maximum number of observers that can be added to the collection.
/// @note This value is defined in the IObserver interface header file.
constexpr uint8_t c_MAX_SENSORCOLLECTION_OBSERVERS = cOBSV_MAX_SENSORCOLLECTION_OBSERVERS; // Maximum number of observers for the sensor collection

/**
 * @brief Class representing a collection of sensors for the SpaceMouse.
 * @details This class implements the ICollection interface and provides functionality for managing a collection of sensors.
 *          It allows adding, removing, and notifying observers of changes in the sensor collection.
 * @note The SensorCollection class is designed to manage a fixed number of sensors and their associated observers.
 */
class SensorCollection : public ICollection {
private:
    // The length of the array is set by the total number keys in the current hardware setup.
    Sensor *m_sensors[cHW_MAX_SENSORS]; // Array of sensor pointers, length is the total number of sensors
    uint8_t m_SensorCount = 0;          // Number of sensor created

    IObserver *m_observers[c_MAX_SENSORCOLLECTION_OBSERVERS]; // Array of observers
    uint8_t m_observerCount = 0;                              // Number of observers attached

public:
    /// @brief Constructor for empty SensorCollection
    SensorCollection();
    ~SensorCollection() {
        for (int i = 0; i < m_SensorCount; i++) {
            delete m_sensors[i]; // Delete each sensor instance to free memory
        }
    }

    /**
     * @brief Set up the sensor collection based on the configuration.
     * @details This function initializes the sensor collection and sets up the sensors based on the configuration.
     *          It creates instances of the sensors and configures them according to the provided configuration.     *
     */
    void setup();

    void evaluate() {
        for (int i = 0; i < m_SensorCount; i++) {
            m_sensors[i]->evaluate();
        }
    }

    // --- Collection management functions --------------------

    void add(ICollectable *sensor) override;
    void remove(ICollectable *sensor) override;

    // --- Addressing items  ---------------------------------

    Sensor *getSensor(const uint8_t id) const;
    Sensor *getSensor(const char *name) const;

    // --- Observer management functions ----------------------

    void attachObserver(IObserver *observer) override { // REFACTOR - Move to Interface!
        if (m_observerCount >= MAX_KEYCOLLECTION_OBSERVERS) {
            // TODO - Handle the case when the observer array is full. Print a message on the serial monitor?
            return;
        }

        // Check if the observer is already attached
        for (uint8_t i = 0; i < m_observerCount; i++) {
            if (m_observers[i] == observer) {
                return; // Observer already attached, do nothing
            }
        }

        m_observers[m_observerCount++] = observer; // Attach the observer to the collection
    }

    void detachObserver(IObserver *observer) override { // REFACTOR - Move to Interface!
        // remove the observer from the array by replacing it with the last observer in the array and decrease the count.
        for (uint8_t i = 0; i < m_observerCount; i++) {
            if (m_observers[i] == observer) {
                m_observerCount--; // Decrease the observer count
                if (m_observerCount > 0) {
                    // Move the last observer to the current position
                    m_observers[i] = m_observers[m_observerCount];
                }
                m_observers[m_observerCount] = nullptr;
                break;
            }
        }
    }
    void notifyObservers() override { // Notify all observers of changes // REFACTOR - Move to Interface!
        for (uint8_t i = 0; i < m_observerCount; i++) {
            m_observers[i]->update(this); // Notify each observer, with the hardware instance as parameter
        }
    }
    void clearObservers() { // REFACTOR - Move to Interface!
        for (uint8_t i = 0; i < m_observerCount; i++) {
            m_observers[i] = nullptr;
        }
        m_observerCount = 0;
    }
};
