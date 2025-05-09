#include "SensorCollection.hpp"

// Include the header file for the sensors that can be added to the Sensor Collection
#include "factory/SensorFactoryHall.hpp"
#include "factory/SensorFactoryJoystick.hpp"

/**
 * @brief Setup the sensor collection according to the configuration.
 * @details This method initializes the sensors based on the configuration defined in config.h.
 *          It creates instances of the sensors and sets their context to this SensorCollection instance.
 * @note The sensor types are mutual exclusive, meaning only one type of sensor can be used.
 *       In order to save compiled code size, preprocessor macros are used to include the correct sensor type.
 * @see config.h for sensor configuration details.
 */
void SensorCollection::setup() {
    for (uint8_t i = 0; i < cHW_MAX_SENSORS; i++) {
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

/**
 * @brief Get the sensor at the specified index.
 * @param id Index of the sensor to retrieve.
 * @return Pointer to the sensor at the specified index, or nullptr if the index is out of bounds.
 * @note The base class Collection::getItem() can be used too, but it returns a pointer to the ICollectable interface.
 */
Sensor *SensorCollection::getSensor(uint8_t id) const {
    return static_cast<Sensor *>(getItem(id)); // Return the sensor with the specified id
}

/**
 * @brief Get the sensor with the specified name.
 * @param name Name of the sensor to retrieve.
 * @return Pointer to the sensor with the specified name, or nullptr if not found.
 * @note The base class Collection::getItem() can be used too, but it returns a pointer to the ICollectable interface.
 */
Sensor *SensorCollection::getSensor(const char *name) const {
    return static_cast<Sensor *>(getItem(name)); // Return the sensor with the specified name
}