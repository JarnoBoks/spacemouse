#include "KeyFactoryPhysicalkey.hpp"

// Include header file for the key to create
#include "key/keys/PhysicalKey.h"

// Include the configuration header file for the key configuration in this factory
#include "config.h"

#include "common/freeRAM.h"

#define NUMBER_OF_PHYSICAL_KEYS NUMKEYS
#define PHYSICALKEY_PINLIST KEY_PINLIST

Key *KeyFactoryPhysicalkey::create(uint8_t id) {

    // Get the pinNumber from the config.h object for this key
    constexpr uint8_t number_of_keys = CFG_NUMBER_OF_KEYS;
    uint8_t key_CFG[number_of_keys][3] = KEYCFG; // Array to hold the key configuration
    FreeRAM::display_freeram();
    Key *newKey = new PhysicalKey(id, key_CFG[id][2]); // Create a new physical key instance, using the id and the pinnumber as defined in the tuple in config.h
    FreeRAM::display_freeram();
    setupFunctionality(newKey); // Set up the functionality for the new key
    FreeRAM::display_freeram();

    return newKey; // Return the new key instance
}
