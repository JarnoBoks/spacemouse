#pragma once

#include "common/Collection.hpp" // Include the ICollection interface header file
#include "common/Observable.hpp" // Include the ICollection interface header file
#include "config.h"              // Include the configuration header file, to retrieve the number of keys (allowed while this is a collection class)
#include <stdint.h>

/// @brief Number of keys that will be added to the collection.
/// @details This is a constant value that defines the maximum number of keys that can be added to the collection. The value is defined in the config.h file.
constexpr uint8_t cNUMBER_OF_KEYS = CFG_NUMBER_OF_KEYS; // Number of keys as defined in config.h

/// @brief Number of observers that can be added to this collection.
/// @details This is a constant value that defines the maximum number of observers that can be added to the collection.
constexpr uint8_t c_MAX_KEYCOLLECTION_OBSERVERS = cNUMBER_OF_KEYS + 4; // Maximum number of observers for the sensor collection

// REVIEW - Should the 'main' routine setup the collection?

/**
 * @brief Class representing a collection of keys for the SpaceMouse.
 * @details This class manages the keys, their states, and observers.
 *          It provides methods to evaluate the keys, get HID commands, and manage observers.
 */
class KeyCollection : public Collection, public Observable {

public:
    /// @brief Constructor for empty KeyCollection
    KeyCollection() : Collection(CFG_NUMBER_OF_KEYS), Observable(c_MAX_KEYCOLLECTION_OBSERVERS) {};
    ~KeyCollection() {}

    void setup();
};
