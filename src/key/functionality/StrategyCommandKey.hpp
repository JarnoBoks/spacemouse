#pragma once
#include "IKeyFunctionality.h"
#include <observers/IObserver.hpp> // Include the IObserver interface header file
#include <key/keys/Key.hpp>

#include <Arduino.h> // Include Arduino library for Serial communication & PROGMEM

/**
 * @brief StrategyCommandKey class handles the execution of HID commands on key press and release events.
 * @details This class implements the IKeyFunctionality interface and provides the functionality to execute a command when a key is pressed or released.
 *          It is used in conjunction with the KeyContext class to manage key events and their associated commands.
 */
class StrategyCommandKey : public IKeyFunctionality {
private:
public:
    StrategyCommandKey() = delete;                                           // Default constructor
    StrategyCommandKey(Key *context_key) : IKeyFunctionality(context_key) {} // Constructor with context

    inline void onPress() override {
        Serial.println(F("StrategyCommandKey::onPress()"));
        IObservable *observable = static_cast<IObservable *>(getContext()); // Get the context of the key
        if (observable)
            observable->notifyObservers(); // Call the notifyObservers method of the key context
    };

    inline void onRelease() override {
        Serial.println(F("StrategyCommandKey::onRelease()"));
        IObservable *observable = static_cast<IObservable *>(getContext()); // Get the context of the key
        if (observable)
            observable->notifyObservers(); // Call the notifyObservers method of the key context
    };
};
