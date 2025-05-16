#include "KeyFactoryPhysicalkey.hpp"

#include "key/keys/PhysicalKey.h"
#include "config.h" // For CFG_NUMBER_OF_KEYS and KEYCFG

#define NUMBER_OF_PHYSICAL_KEYS NUMKEYS
#define PHYSICALKEY_PINLIST KEY_PINLIST

Key *KeyFactoryPhysicalkey::create(uint8_t id) {

    // Get the pinNumber from the config.h object for this key
    constexpr uint8_t number_of_keys = CFG_NUMBER_OF_KEYS;
    uint8_t key_CFG[number_of_keys][3] = KEYCFG; // Array to hold the key configuration

    // Setup the physical key (with the pin number from config.h)
    Key *newKey = new PhysicalKey(id, key_CFG[id][2]);

    // Setup key strategy by calling setupFunctionality function implemented in the KeyFactory base class.
    setupFunctionality(newKey);

    return newKey;
}
