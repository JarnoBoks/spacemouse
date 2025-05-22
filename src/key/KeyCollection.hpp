#pragma once

#include <base/collection/CollectionBase.hpp> // Include the ICollection interface header file
#include <common/Observable.hpp>              // Include the ICollection interface header file
#include <config.h>                           // Include the configuration header file, to retrieve the number of keys (allowed while this is a collection class)
#include <stdint.h>

/// @brief Number of keys that will be added to the collection.
/// @details This is a constant value that defines the maximum number of keys that can be added to the collection. The value is defined in the config.h file.
constexpr uint8_t cNUMBER_OF_KEYS = CFG_NUMBER_OF_KEYS; // Number of keys as defined in config.h

/// @brief Number of observers that can be added to this collection.
/// @details This is a constant value that defines the maximum number of observers that can be added to the collection.
constexpr uint8_t c_MAX_KEYCOLLECTION_OBSERVERS = cNUMBER_OF_KEYS + 4;

class Key;

/**
 * @brief Class representing a collection of keys for the SpaceMouse.
 * @details This class manages the keys, their states, and observers.
 *          It provides methods to evaluate the keys, get HID commands, and manage observers.
 */
class KeyCollection : public CollectionBase, public Observable {

public:
    /// @brief Constructor for empty KeyCollection
    KeyCollection() : CollectionBase(CFG_NUMBER_OF_KEYS), Observable(c_MAX_KEYCOLLECTION_OBSERVERS) {};
    ~KeyCollection() {}

    Key *getKey(const uint8_t id) const;
    Key *getKey(const char *name) const { return nullptr; } // Names are not used in this collection

    /**
     * @brief Evaluate all items in the collection and notify observers of the changes.
     * @details This function iterates through all items in the collection and calls their evaluate method.
     *          This allows each item to perform its own evaluation and update its state accordingly.
     * @note Derived classes are allowed to override this method to provide custom evaluation logic.
     */
    void evaluate() override {
        CollectionBase::evaluate();    // Evaluate the keys in the collection
        Observable::notifyObservers(); // Notify observers of changes in the key collection
    };

    void setup();

    /**
     * @brief Attach an observer to all keys in the collection.
     */
    void attachKeysObserver(IObserver *observer);
};
