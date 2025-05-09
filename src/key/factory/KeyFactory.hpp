#pragma once

#include "key/keys/Key.hpp" // Base class for the keys to create          // REVIEW - Should we use the interface instead of the base class?
// FIXME #include "hidhandler/translator/ITranslator.h"
#include <stdint.h>

/**
 * @brief KeyFactory base class for creating key instances.
 * This class is responsible for creating and managing key instances.
 * It provides methods to create keys, set up their functionality, and evaluate their state.
 */
class KeyFactory {
protected:
    void setupFunctionality(Key *key);

public:
    KeyFactory() = default;
    virtual ~KeyFactory() {};

    virtual Key *create(const uint8_t id) = 0; // Pure virtual function to create a key instance.

    // REVIEW virtual void evaluate() = 0;                  // Abstract from the evaluate() method in Key -> forwarded to the key.
};
