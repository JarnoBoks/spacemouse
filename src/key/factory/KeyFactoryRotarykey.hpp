#include "KeyFactory.hpp"

/**
 * @brief Class for creating rotary key instances.
 * This class inherits from KeyFactory and implements the create method to create physical key instances.
 */
class KeyFactoryRotarykey : public KeyFactory {
private:
public:
    KeyFactoryRotarykey() = default; // Default constructor
    ~KeyFactoryRotarykey() = default;

    Key *create(const uint8_t id) override;
};