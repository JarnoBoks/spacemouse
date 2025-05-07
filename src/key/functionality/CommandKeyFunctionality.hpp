#pragma once
#include "IKeyFunctionality.h"
#include "key/keys/Key.hpp" // Include the Key class to get the key functionality
#include "hardware/IObservable.hpp"
#include "observers/HIDProcessorKey.hpp" // Include the HID processor header for key functionality

class CommandKeyFunctionality : public IKeyFunctionality {
private:
    Key *m_keyContext; // Pointer to the key instance that this functionality is associated with. Always initialized in the constructor.

    void _onChange() {
        // Key is pressed, we have to inform the HID handler that we have a command to send.
        // Attach a key HID observer to the keyCollection.
        if (m_keyContext) {
            // Get the keyCollection from the context of the key object
            IObservable *obsvbl = m_keyContext->getContext();
            if (obsvbl) {
                // Attach the HID processor observer to the KeyCollection, but only if it is not already attached.
                // FIXME - Only when there isn't a HID observer attached for this key yet!
                obsvbl->attachObserver(new HIDProcessorKey());
            }
        }
    }

public:
    CommandKeyFunctionality() = delete; // Default constructor not allowed, keycontext needs to be initialized
    CommandKeyFunctionality(Key *key) : m_keyContext(key) {}

    inline void onPress() override {
        _onChange();
    };

    inline void onRelease() override {
        _onChange();
    };
};
