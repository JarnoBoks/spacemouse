// KeyFactory.h
#pragma once
#include "KeyBase.h"
#include <stdint.h>
#include "config.h"

class KeyFactory {
private:
    KeyBase *keys[NUMKEYS + ROTARY_KEYS];
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
