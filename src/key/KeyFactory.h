// KeyFactory.h
#pragma once
#include "KeyBase.h"
#include <stdint.h>
#include "config.h"

// The total number of keys is defined as:
//      the number of physical keys as defined in config.h (NUMKEYS) +
//      Two rotary keys if they are enabled (ROTARY_KEYS == 1).
constexpr uint8_t NUMBER_OF_KEYS = (NUMKEYS + 2 * ROTARY_KEYS);

class KeyFactory {
private:
    // Setup the array of keys.
    // The length of the array is set by the total number keys in the current hardware setup.
    KeyBase *keys[NUMBER_OF_KEYS];
    int keyCount = 0; // Number of keys created
    static KeyFactory *_instance;

public:
    KeyFactory();
    ~KeyFactory();

    static KeyFactory *getInstance();

    static KeyBase *createKey(KeyType type, KeyConfig *config);

    void evaluate();

    int8_t getKeyCommandsForHID(uint8_t *cmds);

    void setupKeys();
};
