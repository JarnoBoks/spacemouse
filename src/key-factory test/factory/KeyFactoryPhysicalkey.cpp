#include "KeyFactoryPhysicalKey.h"

// Include header file for the key to create
#include "../keys/PhysicalKey.h"

#define NUMBER_OF_PHYSICAL_KEYS NUMKEYS
#define PHYSICALKEY_PINLIST KEY_PINLIST

Key *KeyFactoryPhysicalkey::createKey(uint8_t id) {

    // Get the pinNumber from the config.h object for this key
    constexpr uint8_t number_of_keys = CFG_NUMBER_OF_KEYS;
    uint8_t key_CFG[number_of_keys][3] = KEYCFG; // Array to hold the key configuration

    Key *key = new PhysicalKey(id, key_CFG[id][2]); // Create a new physical key instance

    return key; // Return the new key instance
}
