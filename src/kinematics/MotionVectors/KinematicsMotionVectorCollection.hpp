#pragma once

#include "common/Collection.hpp"  // Include the ICollection interface header file
#include "common/Observable.hpp"  // Include the ICollection interface header file
#include "visitors/Visitable.hpp" // Include the IVisitable interface header file
#include <stdint.h>

// REFACTOR - Create a common base class for all motionvector collections

/// @brief Number of motionvectors that can be added to this collection.
/// @details This is a constant value that defines the maximum number of motionvectors that can be added to the collection.
constexpr uint8_t c_KIN_MAX_MOTIONVECTORS = 3;

/// @brief Number of observers that can be added to this collection.
/// @details This is a constant value that defines the maximum number of observers that can be added to the collection.
constexpr uint8_t c_KIN_MAX_MOTIONVECTORCOLLECTION_OBSERVERS = 4;

/**
 * @brief Class representing a visitable,observable collection of motionvectors for the SpaceMouse Kinematics
 * @details The class implements the ICollection interface and provides functionality for managing a collection of motionvectors.
 *          The class implements the Observable interface, allowing it to notify observers of changes in the collection.
 * The class implements the Visitable interface, allowing it to accept visitors.
 * @note The KinematicsMotionVectorCollection class is designed to manage a fixed number of motionvectors and their associated observers.
 */
class KinematicsMotionVectorCollection : public Collection, public Observable, public Visitable {
private:
    int16_t m_totalVelocity; // Total velocity of the motion vectors in the collection
public:
    /// @brief Constructor for empty KinematicsMotionVectorCollection
    KinematicsMotionVectorCollection()
        : Collection(c_KIN_MAX_MOTIONVECTORS),
          Observable(c_KIN_MAX_MOTIONVECTORCOLLECTION_OBSERVERS) {}
    ~KinematicsMotionVectorCollection() {
    }

    void execute() {
        // Execute the kinematics motion vector collection
        // This function should contain the logic to process the motion vectors
    }
};