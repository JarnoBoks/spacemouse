#include "KeyFactoryRotarykey.h" // Include the header file for the key factory

// Include header file for the key to create
#include "../keys/RotaryKey.h"

// Include header files for key functionalities
#include "../functionality/KillRotationFunctionality.h"    // For KillRotationFunctionality
#include "../functionality/KillTranslationFunctionality.h" // For KillRotationFunctionality
#include "../functionality/CommandKeyFunctionality.h"      // For KillSwitchKeyFunctionality

Key *KeyFactoryRotarykey::createKey(const uint8_t id) {
    Key *key = new RotaryKey(id); // Create a new physical key instance
    setupFunctionality(key);      // Set up the functionality for the key

    return key; // Return the new key instance
}
