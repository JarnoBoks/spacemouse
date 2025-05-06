#include "config.h"
#include <stdint.h>

// #include "keys/Key.h"
#include "factory/KeyFactoryPhysicalkey.h"
#include "factory/KeyFactoryRotarykey.h"

// Defines to retrieve the key configuration from config.h
constexpr uint8_t cNUMBER_OF_KEYS = CFG_NUMBER_OF_KEYS;
uint8_t cKEY_CONFIGS[cNUMBER_OF_KEYS][3] = KEYCFG; // Array to hold the key configuration

#define KEY_CFG_TYPE cKEY_CONFIGS[i][0] // Type of key (PHYSICAL or ROTARY)
#define KEY_CFG_FUNC cKEY_CONFIGS[i][1] // Button type (SM_T, SM_R, etc.)
#define KEY_CFG_PINN cKEY_CONFIGS[i][2] // Pin number

// REFACTOR - The constructor code should be moved to the Collection Factory class, which is not implemented yet.
// REVIEW - Should the 'main' routine setup the collection?

class KeyHandler {
private:
    // The length of the array is set by the total number keys in the current hardware setup.
    Key *keys[cNUMBER_OF_KEYS] = {nullptr}; // Array of key pointers, length is the total number of keys
    int m_keyCount = 0;                     // Number of keys created

public:
    KeyHandler() {

        for (int i = 0; i < cNUMBER_OF_KEYS; i++) {
            // The first element of each key contains the type of key (PHYSICAL or ROTARY).
            // The second element contains the button type (SM_T, SM_R, etc.)
            // The third element contains the pin number if applicable.

            if (KEY_CFG_TYPE == KEY_PHYSICAL) {
                KeyFactoryPhysicalkey factory;                    // Create a factory for physical keys
                keys[m_keyCount] = factory.createKey(m_keyCount); // Create a new key instance using the factory
                m_keyCount++;

            } else if (KEY_CFG_TYPE == KEY_ROTARY) {
                KeyFactoryRotarykey factory;                      // Create a factory for rotary keys
                keys[m_keyCount] = factory.createKey(m_keyCount); // Create a new key instance using the factory
                m_keyCount++;

            } else {
                // Invalid key type, handle error or skip
                continue; // Skip to the next iteration if the key type is not recognized
            }
        }
    };

    ~KeyHandler() {
        for (int i = 0; i < m_keyCount; i++) {
            delete keys[i]; // Delete each key instance to free memory
            m_keyCount = 0;
        }
    } // TODO

    void evaluate();

    int8_t KeyHandler::getHIDcommands(uint8_t *cmds);

    inline void addKey(Key *key) {
        if (m_keyCount < cNUMBER_OF_KEYS) {
            keys[m_keyCount++] = key; // Add the key to the list of keys
        }
    }

    inline void removeKey(Key *key) {
        for (int i = 0; i < m_keyCount; i++) {
            if (keys[i] == key) {
                m_keyCount--; // Decrease the key count
                if (m_keyCount > 0) {
                    // Move the last key to the current position
                    keys[i] = keys[m_keyCount];
                }
                keys[m_keyCount] = nullptr;
                break;
            }
        }
    }
};
