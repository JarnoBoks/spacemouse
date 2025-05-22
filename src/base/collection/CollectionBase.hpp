#pragma once

#include <common/ICollection.hpp>
#include <common/ICollectable.hpp>

#include <stdint.h>
// REFACTOR - Split to hpp and cpp file.
/**
 * @brief Base class representing a collection of items for the SpaceMouse.
 * @details Implements ICollection and provides functionality for managing a collection of axes.
 *          It has methods for adding, removing, retrieving, evaluating of collection items, as well as methods for
 *          and notifying observers of (all) items in the collection.
 * @note Used to store keys, sensors, knob- and kinematics axes.
 */
class CollectionBase : public ICollection {
private:
    const uint8_t m_maxItems = 0; // Maximum number of items in the collection
protected:
    ICollectable **m_items;
    uint8_t m_itemCount = 0; // Number of items created
public:
    /// @brief Constructor for empty CollectionBase
    CollectionBase() = delete; // Delete the default constructor to prevent instantiation without parameters
    CollectionBase(uint8_t maxItems);
    virtual ~CollectionBase();

    // --- CollectionBase management functions --------------------
    void add(ICollectable *item) override;
    void remove(ICollectable *item) override;

    virtual void evaluate();

    // --- Addressing items  ---------------------------------

    ICollectable *getItem(const uint8_t id) const override;
    ICollectable *getItem(const char *descriptor) const override;

    uint8_t getItemCount() const override;
};
