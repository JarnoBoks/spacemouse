#include "KeyCollection.hpp"
#include <Arduino.h> // Include Arduino library for Serial  function

constexpr uint8_t cKEY_CONFIGS[cNUMBER_OF_KEYS][3] = KEYCFG; // Array to hold the key configuration

#define KEY_CFG_TYPE cKEY_CONFIGS[i][0] // Type of key (PHYSICAL or ROTARY)
#define KEY_CFG_FUNC cKEY_CONFIGS[i][1] // Button type (SM_T, SM_R, etc.)
#define KEY_CFG_PINN cKEY_CONFIGS[i][2] // Pin number

/**
 * @brief Constructor for KeyCollection class.
 * @details This constructor initializes an empty key collection.
 */
KeyCollection::KeyCollection() {
    for (uint8_t i = 0; i < cNUMBER_OF_KEYS; i++) {
        m_keys[i] = nullptr;
    }
    for (uint8_t i = 0; i < MAX_KEYCOLLECTION_OBSERVERS; i++) {
        m_observers[i] = nullptr;
    }
}

/**
 * @brief Setup the key collection according to the configuration.
 * @details This method initializes the keys based on the configuration defined in config.h.
 *          It creates instances of the keys and sets their context to this KeyCollection instance.
 * @see config.h for key configuration details.
 */
void KeyCollection::setup() {
    for (int i = 0; i < cNUMBER_OF_KEYS; i++) {
        // The first element of each key contains the type of key (PHYSICAL or ROTARY).
        // The second element contains the button type (SM_T, SM_R, etc.)
        // The third element contains the pin number if applicable.

        if (cKEY_CONFIGS[i][0] == KEY_PHYSICAL) {
            KeyFactoryPhysicalkey factory;                   // Create a factory for physical keys
            m_keys[m_keyCount] = factory.create(m_keyCount); // Create a new key instance using the factory

        } else if (cKEY_CONFIGS[i][0] == KEY_ROTARY) {
            KeyFactoryRotarykey factory;                     // Create a factory for rotary keys
            m_keys[m_keyCount] = factory.create(m_keyCount); // Create a new key instance using the factory

        } else {
            // Invalid key type, handle error or skip
            continue; // Skip to the next iteration if the key type is not recognized
        }

        m_keys[m_keyCount]->setContext(this); // Set the context of the key instance to this KeyCollection instance
        m_keyCount++;
    }
};

void KeyCollection::add(Key *key) {
    if (m_keyCount < cNUMBER_OF_KEYS) {
        m_keys[m_keyCount++] = key; // Add the key to the list of keys
    }
}

void KeyCollection::remove(Key *key) {
    for (int i = 0; i < m_keyCount; i++) {
        if (m_keys[i] == key) {
            m_keyCount--; // Decrease the key count
            if (m_keyCount > 0) {
                // Move the last key to the current position
                m_keys[i] = m_keys[m_keyCount];
            }
            m_keys[m_keyCount] = nullptr;
            break;
        }
    }
}

#if 0
/**
 * @brief Retrieves the key commands for the HID
 * @param cmds Pointer to the array where the commands will be stored
 * @return The number of commands that have to be sent
 * @deprecated
 */
int8_t KeyCollection::getHIDcommands(uint8_t *cmds) {
    uint8_t result_idx = 0;
    for (int i = 0; i < m_keyCount; i++) {
        result_idx += m_keys[i]->getHIDCommand(cmds + result_idx); // Get the HID command for each key
    }

    return result_idx; // Return the number of commands that have to be sent
}
#endif
