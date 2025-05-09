#include "SensorFactory.hpp"

/**
 * @brief Class for creating hall sensor instances.
 * This class inherits from SensorFactory and implements the create method to create hall sensor instances.
 */
class SensorFactoryHall : public SensorFactory {
private:
public:
    SensorFactoryHall() = default;
    ~SensorFactoryHall() {};

    Sensor *create(const uint8_t id) override;
};