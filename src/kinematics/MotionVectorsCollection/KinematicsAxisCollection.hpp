#pragma once

#include <common/Observable.hpp>                 // Include the ICollection interface header file
#include <collection/MotionVectorCollection.hpp> // Base class for the collection of motion vectors
#include <visitors/Visitable.hpp>                // Include the IVisitable interface header file
#include <stdint.h>

#include <motionvector/MotionVectorType.h> // For MotionVector_t enum
class KinematicsAxis;

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
 * @note The KinematicsAxisCollection class is designed to manage a fixed number of motionvectors and their associated observers.
 */
class KinematicsAxisCollection : public MotionVectorCollection, public Observable, public Visitable {
private:
    int16_t m_totalVelocity; // Total velocity of the motion vectors in the collection
public:
    /// @brief Constructor for empty KinematicsAxisCollection
    KinematicsAxisCollection()
        : MotionVectorCollection(c_KIN_MAX_MOTIONVECTORS),
          Observable(c_KIN_MAX_MOTIONVECTORCOLLECTION_OBSERVERS) {}
    ~KinematicsAxisCollection() = default;

    void evaluate() override final;

    const int16_t getTotalVelocity() const { return m_totalVelocity; }; // Getter for the total velocity of the motion vectors in the collection

    void setAllToZero(); // Set all motion vectors in the collection to zero
};