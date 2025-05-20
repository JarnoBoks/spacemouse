#pragma once

#include "common/Collection.hpp"  // Include the ICollection interface header file
#include "common/Observable.hpp"  // Include the ICollection interface header file
#include "visitors/Visitable.hpp" // Include the IVisitable interface header file
#include <stdint.h>

#include <motionvector/MotionVectorType.h> // For MotionVector_t enum
class KinematicsMotionVector;

/// @brief Number of Kinematics MotionVectors that can be added to this collection.
/// @details This is a constant value that defines the maximum number of MotionVectors that can be added to the collection.
constexpr uint8_t c_KIN_MAX_MOTIONVECTORS = 3;

/// @brief Number of observers that can be added to this collection.
/// @details This is a constant value that defines the maximum number of observers that can be added to the collection.
constexpr uint8_t c_KIN_MAX_MOTIONVECTORCOLLECTION_OBSERVERS = 4;

/**
 * @brief Class representing a visitable,observable collection of MotionVectors for the SpaceMouse Kinematics
 * @details The class implements the ICollection interface and provides functionality for managing a collection of MotionVectors.
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
    ~KinematicsMotionVectorCollection() = default;

    void evaluate() override final;

    const int16_t getTotalVelocity() const { return m_totalVelocity; }; // Getter for the total velocity of the motion vectors in the collection

    // Extend the Collection class to add a function to get the MotionVector by type
    KinematicsMotionVector *getItem(MotionVector_t type) const;

    void setAllToZero(); // Set all motion vectors in the collection to zero
};