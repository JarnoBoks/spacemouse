#pragma once

#include <collection/Collection.hpp>
#include <motionvector/MotionVectorType.h> // For MotionVector_t enum
#include <motionvector/MotionVector.hpp>   // Include the header file for MotionVector class

/**
 * @brief Base class representing a collection of MotionVectors for the SpaceMouse.
 * @details This class is derived from the Collection class and extends her functionality for retrieving an item based on its motion vector type.
 */
class MotionVectorCollection : public Collection {
public:
    MotionVectorCollection(uint8_t maxItems) : Collection(maxItems) {}
    virtual ~MotionVectorCollection() {};

    using Collection::getItem; // Bring the base class getItem function into the derived class
    ICollectable *getItem(MotionVector_t type) const {
        for (uint8_t i = 0; i < m_itemCount; i++) {
            if (m_items[i] != nullptr && (static_cast<MotionVector *>(m_items[i])->getType() == type)) {
                return m_items[i];
            }
        }
        return nullptr; // Return nullptr if no matching sensor is found
    }
};