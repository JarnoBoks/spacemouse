#include "SensorFactory.hpp"

/**
 * @brief Class for creating joystick sensor instances.
 * This class inherits from SensorFactory and implements the create method to create joystick sensor instances.
 */
class SensorFactoryJoystick : public SensorFactory {
private:
public:
    SensorFactoryJoystick() = default;
    ~SensorFactoryJoystick() {};

    Sensor *create(const uint8_t id) override;
};