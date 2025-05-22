#include "DirectionalMotionVectorCollection.hpp"
#include <kinematics/MotionVector/KinematicsMotionVector.hpp>
#include <Arduino.h> // For abs()

/**
 * @brief Evaluate all items in the collection and notify observers of the changes.
 * @details This function iterates through all items in the collection and calls their evaluate method.
 *          This allows each item to perform its own evaluation and update its state accordingly.
 * @note Derived classes are allowed to override this method to provide custom evaluation logic.
 */
void KinematicsMotionVectorCollection::evaluate() {
    m_totalVelocity = 0; // Reset total velocity before evaluation

    // Evaluate all items (ie. KinematicsMotionVectors) in the collection.
    Collection::evaluate();

    // Calculate the total velocity of the motion vectors
    for (uint8_t i = 0; i < m_itemCount; i++) {
        m_totalVelocity += abs(static_cast<KinematicsMotionVector *>(m_items[i])->getFinValue());
    }

    // Notify observers of changes in the kinematicsVector collection
    Observable::notifyObservers();
};

void KinematicsMotionVectorCollection::setAllToZero() {
    for (uint8_t i = 0; i < m_itemCount; i++) {
        static_cast<KinematicsMotionVector *>(m_items[i])->setFinValue(0); // Set the final value of each motion vector to 0
    }
}