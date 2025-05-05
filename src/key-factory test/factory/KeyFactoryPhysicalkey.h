#include "KeyFactory.h"

/**
 * @brief Class for creating physical key instances.
 * This class inherits from KeyFactory and implements the createKey method to create physical key instances.
 */
class KeyFactoryPhysicalkey : public KeyFactory {
private:
public:
    KeyFactoryPhysicalkey() = default;
    ~KeyFactoryPhysicalkey() {};

    Key *createKey(const uint8_t id) override;
};