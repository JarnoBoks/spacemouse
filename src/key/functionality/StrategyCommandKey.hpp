#pragma once
#include "StrategyBase.hpp"
#include <observers/IObserver.hpp> // Include the IObserver interface header file
#include <key/keys/Key.hpp>

#include <Arduino.h> // Include Arduino library for Serial communication & PROGMEM

/**
 * @brief StrategyCommandKey class handles the execution of HID commands on key press and release events.
 * @details This class implements the StrategyBase interface and provides the functionality to execute a command when a key is pressed or released.
 *          It is used in conjunction with the KeyContext class to manage key events and their associated commands.
 */
class StrategyCommandKey : public StrategyBase {
private:
public:
    StrategyCommandKey() = delete;
    StrategyCommandKey(Key *context_key) : StrategyBase(context_key) {} // Constructor with context

    /**
     * @brief Handles the key press event.
     * @details This method is called when the key is pressed.
     *          It retrieves the key from the strategy context and notifies the observers associated with the key.
     */
    inline void onPress() override {
        IObservable *observable = static_cast<IObservable *>(getContext()); // Get the  key
        if (observable)
            observable->notifyObservers();
    };

    /**
     * @brief Handles the key release event.
     * @details This method is called when the key is released.
     *          It retrieves the key from the strategy context and notifies the observers associated with the key.
     */
    inline void onRelease() override {
        IObservable *observable = static_cast<IObservable *>(getContext()); // Get the key
        if (observable)
            observable->notifyObservers();
    };
};
