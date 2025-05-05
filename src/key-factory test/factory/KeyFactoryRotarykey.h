#include "KeyFactory.h"

/**
 * @brief Class for creating rotary key instances.
 * This class inherits from KeyFactory and implements the createKey method to create physical key instances.
 */
class KeyFactoryRotarykey : public KeyFactory {
private:
public:
    KeyFactoryRotarykey() = default; // Default constructor
    ~KeyFactoryRotarykey() = default;

    Key *createKey(const uint8_t id) override;
};