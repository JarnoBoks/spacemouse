#include "config.h" // Allowed here while this is a Collection Factory class.

#include "hardware/IObservable.hpp" // Include the IObservable interface header file
#include "observers/IObserver.hpp"  // Include the IObserver interface header file

#include "keys/Key.hpp" // Include the Key class header file

#include "factory/KeyFactoryPhysicalkey.h" // TODO - Move to cpp
#include "factory/KeyFactoryRotarykey.h"   // TODO - Move to cpp

// Defines to retrieve the key configuration from config.h
constexpr uint8_t cNUMBER_OF_KEYS = CFG_NUMBER_OF_KEYS;
uint8_t cKEY_CONFIGS[cNUMBER_OF_KEYS][3] = KEYCFG; // Array to hold the key configuration

#define KEY_CFG_TYPE cKEY_CONFIGS[i][0] // Type of key (PHYSICAL or ROTARY)
#define KEY_CFG_FUNC cKEY_CONFIGS[i][1] // Button type (SM_T, SM_R, etc.)
#define KEY_CFG_PINN cKEY_CONFIGS[i][2] // Pin number

#include <stdint.h>

// REFACTOR - The constructor code should be moved to the Collection Factory class, which is not implemented yet.
// REVIEW - Should the 'main' routine setup the collection?

class KeyCollection : IObservable {
private:
    // The length of the array is set by the total number keys in the current hardware setup.
    Key *m_keys[cNUMBER_OF_KEYS] = {nullptr}; // Array of key pointers, length is the total number of keys
    uint8_t m_keyCount = 0;                   // Number of keys created

    IObserver *m_observers[MAX_KEYCOLLECTION_OBSERVERS] = {nullptr}; // Array of observers
    uint8_t m_observerCount = 0;                                     // Number of observers attached

public:
    KeyCollection() {

        for (int i = 0; i < cNUMBER_OF_KEYS; i++) {
            // The first element of each key contains the type of key (PHYSICAL or ROTARY).
            // The second element contains the button type (SM_T, SM_R, etc.)
            // The third element contains the pin number if applicable.

            if (KEY_CFG_TYPE == KEY_PHYSICAL) {
                KeyFactoryPhysicalkey factory;                      // Create a factory for physical keys
                m_keys[m_keyCount] = factory.createKey(m_keyCount); // Create a new key instance using the factory

            } else if (KEY_CFG_TYPE == KEY_ROTARY) {
                KeyFactoryRotarykey factory;                        // Create a factory for rotary keys
                m_keys[m_keyCount] = factory.createKey(m_keyCount); // Create a new key instance using the factory

            } else {
                // Invalid key type, handle error or skip
                continue; // Skip to the next iteration if the key type is not recognized
            }

            m_keys[m_keyCount]->setContext(this); // Set the context of the key instance to this KeyCollection instance
            m_keyCount++;
        }
    };

    ~KeyCollection() {
        for (int i = 0; i < m_keyCount; i++) {
            delete m_keys[i]; // Delete each key instance to free memory
            m_keyCount = 0;
        }
    } // TODO - Should the constructor code be moved to the main routine?

    void evaluate();

    int8_t getHIDcommands(uint8_t *cmds);

    inline void addKey(Key *key) {
        if (m_keyCount < cNUMBER_OF_KEYS) {
            m_keys[m_keyCount++] = key; // Add the key to the list of keys
        }
    }

    inline void removeKey(Key *key) {
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

    void attachObserver(IObserver *observer) { // REFACTOR - Move to Interface!
        if (m_observerCount >= MAX_KEYCOLLECTION_OBSERVERS) {
            // TODO - Handle the case when the observer array is full. Print a message on the serial monitor?
            return;
        }

        // Check if the observer is already attached
        for (uint8_t i = 0; i < m_observerCount; i++) {
            if (m_observers[i] == observer) {
                return; // Observer already attached, do nothing
            }
        }

        m_observers[m_observerCount++] = observer; // Attach the observer to the collection
    }

    void detachObserver(IObserver *observer) { // REFACTOR - Move to Interface!
        // remove the observer from the array by replacing it with the last observer in the array and decrease the count.
        for (uint8_t i = 0; i < m_observerCount; i++) {
            if (m_observers[i] == observer) {
                m_observerCount--; // Decrease the observer count
                if (m_observerCount > 0) {
                    // Move the last observer to the current position
                    m_observers[i] = m_observers[m_observerCount];
                }
                m_observers[m_observerCount] = nullptr;
                break;
            }
        }
    }
    void notifyObservers() { // Notify all observers of changes // REFACTOR - Move to Interface!
        for (uint8_t i = 0; i < m_observerCount; i++) {
            m_observers[i]->update(this); // Notify each observer, with the hardware instance as parameter
        }
    }
    void clearObservers() { // REFACTOR - Move to Interface!
        for (uint8_t i = 0; i < m_observerCount; i++) {
            m_observers[i] = nullptr;
        }
        m_observerCount = 0;
    }
};
