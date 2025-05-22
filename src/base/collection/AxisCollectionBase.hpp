#pragma once

#include <base/collection/CollectionBase.hpp>
#include <base/axis/MotionVectorType.h> // For MotionVector_t enum
#include <base/axis/AxisBase.hpp>       // Include the header file for MotionVector class

/**
 * @brief Base class representing a collection of axes for the SpaceMouse.
 * @details This class is derived from the CollectionBase class and extends her functionality for retrieving an item based on its motion vector type.
 */
class AxisCollectionBase : public CollectionBase {
public:
    AxisCollectionBase(uint8_t maxItems) : CollectionBase(maxItems) {}
    virtual ~AxisCollectionBase() {};

    using CollectionBase::getItem; // Bring the base class getItem function into the derived class
    ICollectable *getItem(MotionVector_t type) const {
        for (uint8_t i = 0; i < m_itemCount; i++) {
            if (m_items[i] != nullptr && (static_cast<AxisBase *>(m_items[i])->getType() == type)) {
                return m_items[i];
            }
        }
        return nullptr; // Return nullptr if no matching sensor is found
    }
};