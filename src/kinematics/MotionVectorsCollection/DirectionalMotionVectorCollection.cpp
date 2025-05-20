#include "DirectionalMotionVectorCollection.hpp"

#include <kinematics/MotionVector/KinematicsMotionVector.hpp>
/**
 * @brief Evaluate all items in the collection and notify observers of the changes.
 * @details This function iterates through all items in the collection and calls their evaluate method.
 *          This allows each item to perform its own evaluation and update its state accordingly.
 * @note Derived classes are allowed to override this method to provide custom evaluation logic.
 */
void KinematicsMotionVectorCollection::evaluate() {
    Collection::evaluate(); // Evaluate all items (ie. KinematicsMotionVectors) in the collection.

    // Calculate the total velocity of the motion vectors
    for (uint8_t i = 0; i < m_itemCount; i++) {
        m_totalVelocity += static_cast<KinematicsMotionVector *>(m_items[i])->getFinValue();
    }
    Observable::notifyObservers(); // Notify observers of changes in the axis collection
};

KinematicsMotionVector *KinematicsMotionVectorCollection::getItem(MotionVector_t type) const {
    for (uint8_t i = 0; i < getItemCount(); i++) {
        KinematicsMotionVector *motionVector = static_cast<KinematicsMotionVector *>(Collection::getItem(i));
        if (motionVector != nullptr && motionVector->getType() == type) {
            return motionVector;
        }
    }
    return nullptr; // Return nullptr if no matching sensor is found
}