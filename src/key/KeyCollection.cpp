#include "KeyCollection.hpp"

// Include concrete factories for the keys
#include "factory/KeyFactoryPhysicalkey.hpp" // Include the factory for physical keys
#include "factory/KeyFactoryRotarykey.hpp"   // Include the factory for rotary keys
#include "config.h"                          // Include the configuration header file, to retrieve the number of keys (allowed while this is a collection class)

#include <Arduino.h> // Include Arduino library for Serial  function

constexpr uint8_t cKEY_CONFIGS[cNUMBER_OF_KEYS][3] = KEYCFG; // Array to hold the key configuration, as defined in config.h

#define KEY_CFG_TYPE cKEY_CONFIGS[i][0] // Type of key (PHYSICAL or ROTARY)
#define KEY_CFG_FUNC cKEY_CONFIGS[i][1] // Button type (SM_T, SM_R, etc.)
#define KEY_CFG_PINN cKEY_CONFIGS[i][2] // Pin number

/**
 * @brief Setup the key collection according to the configuration.
 * @details This method initializes the keys based on the configuration defined in config.h.
 *          It creates instances of the keys and sets their context to this KeyCollection instance.
 * @see config.h for key configuration details.
 */
void KeyCollection::setup() {
    for (uint8_t i = 0; i < cNUMBER_OF_KEYS; i++) {
        // The first element of each key contains the type of key (PHYSICAL or ROTARY).
        // The second element contains the button type (SM_T, SM_R, etc.)
        // The third element contains the pin number if applicable.

        KeyFactory *factory; // Pointer to the factory for creating keys
        if (cKEY_CONFIGS[i][0] == KEY_PHYSICAL) {
            KeyFactoryPhysicalkey factory;
            m_items[m_itemCount] = factory.create(m_itemCount);

        } else if (cKEY_CONFIGS[i][0] == KEY_ROTARY) {
            KeyFactoryRotarykey factory;
            m_items[m_itemCount] = factory.create(m_itemCount);
        } else {
            // Invalid key type, handle error or skip
            continue; // Skip to the next iteration if the key type is not recognized
        }
        m_items[m_itemCount]->setContext(this); // Set the context of the key instance to this KeyCollection instance
        m_itemCount++;
        delete factory; // Delete the factory instance to free memory
    }
};

#undef KEY_CFG_TYPE
#undef KEY_CFG_FUNC
#undef KEY_CFG_PINN

// REVIEW Can we use a template function in the base class for all these getItem functions in the collection classes?
Key *KeyCollection::getKey(const uint8_t id) const {
    return static_cast<Key *>(getItem(id)); // Call the base class method to get the item at the specified index
}

/**
 * @brief   Attach an observer to all keys in the collection.
 * @details This function iterates through all keys in the collection and attaches the provided observer to each key.
 *          This allows the observer to receive updates from all keys in the collection.
 * @param observer Pointer to the observer to be attached.
 */
void KeyCollection::attachKeysObserver(IObserver *observer) {
    for (int i = 0; i < m_itemCount; i++) {
        static_cast<Key *>(m_items[i])->attachObserver(observer); // Attach the observer to each key in the collection
    }
};
