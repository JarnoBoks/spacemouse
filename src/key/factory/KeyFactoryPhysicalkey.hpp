#include "KeyFactory.hpp"

/**
 * @brief Class for creating physical key instances.
 * This class inherits from KeyFactory and implements the create method to create physical key instances.
 */
class KeyFactoryPhysicalkey : public KeyFactory {
private:
public:
    KeyFactoryPhysicalkey() = default;
    ~KeyFactoryPhysicalkey() {};

    Key *create(const uint8_t id) override;
};