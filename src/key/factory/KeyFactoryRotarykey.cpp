#include "KeyFactoryRotarykey.h" // Include the header file for the key factory

// Include header file for the key to create
#include "key/keys/RotaryKey.h"

Key *KeyFactoryRotarykey::createKey(const uint8_t id) {
    Key *newKey = new RotaryKey(id); // Create a new physical key instance
    setupFunctionality(newKey);      // Set up the functionality for the key

    return newKey; // Return the new key instance
}
