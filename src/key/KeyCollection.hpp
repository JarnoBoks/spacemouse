#pragma once

#include "config.h" // Allowed here while this is a Collection Factory class.
#include <stdint.h>
// Defines to retrieve the key configuration from config.h
constexpr uint8_t cNUMBER_OF_KEYS = CFG_NUMBER_OF_KEYS;

#include "hardware/IObservable.hpp" // Include the IObservable interface header file
#include "observers/IObserver.hpp"  // Include the IObserver interface header file

#include "keys/Key.hpp" // Include the Key class header file

#include "factory/KeyFactoryPhysicalkey.h" // TODO - Move to cpp
#include "factory/KeyFactoryRotarykey.h"   // TODO - Move to cpp

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
    KeyCollection();
    ~KeyCollection() {
        for (int i = 0; i < m_keyCount; i++) {
            delete m_keys[i]; // Delete each key instance to free memory
            m_keyCount = 0;
        }
    } // TODO - Should the constructor code be moved to the main routine?

    inline void evaluate() {
        for (int i = 0; i < m_keyCount; i++) {
            m_keys[i]->evaluate();
        }
    }

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
