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
