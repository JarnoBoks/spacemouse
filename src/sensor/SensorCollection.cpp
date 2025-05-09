#include "SensorCollection.hpp"

// Include the header file for the sensors that can be added to the Sensor Collection
#include "factory/SensorFactoryHall.hpp"
#include "factory/SensorFactoryJoystick.hpp"

#if 0 // REMOVE - Implemented by the base classes Collection & Observable
/**
 * @brief Constructor for KeyCollection class.
 * @details This constructor initializes an empty key collection.
 */
SensorCollection::SensorCollection() {
    // Constructor for KeyCollection class
    for (uint8_t i = 0; i < cHW_MAX_SENSORS; i++) {
        m_sensors[i] = nullptr; // Initialize each key pointer to nullptr
    }
    for (uint8_t i = 0; i < c_MAX_SENSORCOLLECTION_OBSERVERS; i++) {
        m_observers[i] = nullptr; // Initialize each observer pointer to nullptr
    }
}
#endif

/**
 * @brief Setup the sensor collection according to the configuration.
 * @details This method initializes the sensors based on the configuration defined in config.h.
 *          It creates instances of the sensors and sets their context to this SensorCollection instance.
 * @note The sensor types are mutual exclusive, meaning only one type of sensor can be used.
 *       In order to save compiled code size, preprocessor macros are used to include the correct sensor type.
 * @see config.h for sensor configuration details.
 */
void SensorCollection::setup() {
    for (int i = 0; i < cHW_MAX_SENSORS; i++) {
#ifdef HW_HALLEFFECT
        SensorFactoryHall factory; // Create a factory for Hall effect sensors
#else
        SensorFactoryJoystick factory; // [DEFAULT] Create a factory for Joystick sensors
#endif
        m_items[m_itemCount] = factory.create(m_itemCount); // Create a new sensor instance using the factory
        m_items[m_itemCount]->setContext(this);             // Set the context of the key instance to this SensorCollection instance
        m_itemCount++;
    }
};

#if 0
void SensorCollection::add(ICollectable *sensor) {
    if (m_SensorCount < cHW_MAX_SENSORS) {
        m_sensors[m_SensorCount++] = static_cast<Sensor *>(sensor); // Add the key to the list of keys
    }
}

void SensorCollection::remove(ICollectable *sensor) {
    for (int i = 0; i < m_SensorCount; i++) {
        if (m_sensors[i] == static_cast<Sensor *>(sensor)) {
            m_SensorCount--; // Decrease the sensor count
            if (m_SensorCount > 0) {
                // Move the last sensor to the current position
                m_sensors[i] = m_sensors[m_SensorCount];
            }
            m_sensors[m_SensorCount] = nullptr;
            break;
        }
    }
}
#endif
#if 0 // REMOVE - Implemented by the base classes Collection & Observable

Sensor *SensorCollection::getSensor(const char *name) const {
    for (uint8_t i = 0; i < m_SensorCount; i++) {
        if (m_sensors[i] != nullptr && m_sensors[i]->isCurrentSensor(name)) {
            return m_sensors[i];
        }
    }
    return nullptr; // Return nullptr if no matching sensor is found
};

Sensor *SensorCollection::getSensor(const uint8_t id) const {
    if (id < m_SensorCount) {
        return m_sensors[id]; // Return the sensor at the specified index
    }
    return nullptr; // Return nullptr if the index is out of bounds
}
#endif