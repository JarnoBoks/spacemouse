// KeyFactory.h
#pragma once
#include "../keys/Key.h" // Base class for the keys to create
#include <stdint.h>
#include "config.h"

/**
 * @brief KeyFactory class for creating key instances.
 * This class is responsible for creating and managing key instances.
 * It provides methods to create keys, set up their functionality, and evaluate their state.
 */
class KeyFactory {
protected:
    void setupFunctionality(Key *key);

public:
    KeyFactory();
    virtual ~KeyFactory();

    virtual Key *createKey(const uint8_t id) = 0; // Pure virtual function to create a key instance.
    // REVIEW virtual void evaluate() = 0;                  // Abstract from the evaluate() method in Key -> forwarded to the key.
};
