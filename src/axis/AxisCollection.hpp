#pragma once

#include "common/Collection.hpp" // Include the ICollection interface header file
#include "common/Observable.hpp" // Include the ICollection interface header file

#include <stdint.h>

/// @brief Number of axes that can be added to the collection.
/// @details This is a constant value that defines the maximum number of axes that can be added to the collection.
/// @note As long as the specific hardware options for the SpaceMouse all have the same number of axes, this is a good solution.
constexpr uint8_t cHW_MAX_AXES = 6;

/// @brief Number of observers that can be added to this collection.
/// @details This is a constant value that defines the maximum number of observers that can be added to the collection.
/// @note This value is defined in the IObserver interface header file.
constexpr uint8_t c_MAX_AXISCOLLECTION_OBSERVERS = 4; // Maximum number of observers for the axis collection

/**
 * @brief Class representing a collection of axes for the SpaceMouse.
 * @details This class implements the ICollection interface and provides functionality for managing a collection of axes.
 *          It allows adding, removing, and notifying observers of changes in the sensor collection.
 * @note The AxisCollection class is designed to manage a fixed number of axes and their associated observers.
 */
class AxisCollection : public Collection, public Observable {
private:
public:
    /// @brief Constructor for empty AxisCollection
    AxisCollection() : Collection(cHW_MAX_AXES), Observable(c_MAX_AXISCOLLECTION_OBSERVERS) {}
    ~AxisCollection() {
    }

    /**
     * @brief Set up the sensor collection based on the configuration.
     * @details This function initializes the sensor collection and sets up the axes based on the configuration.
     *          It creates instances of the axes and configures them according to the provided configuration.     *
     */
    void setup();
};
