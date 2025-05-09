#pragma once

#include "ICollection.hpp"
#include "ICollectable.hpp" // Include the ICollectable interface header file

#include <stdint.h>

/**
 * @brief Base class representing a collection of items for the SpaceMouse.
 * @details This class implements the ICollection interface and provides functionality for managing a collection of axes.
 *          It allows adding, removing, and notifying observers of changes in the sensor collection.
 */
class Collection : public ICollection {
private:
    uint8_t m_maxItems = 0; // Maximum number of items in the collection
protected:
    ICollectable **m_items;
    uint8_t m_itemCount = 0; // Number of sensor created
public:
    /// @brief Constructor for empty Collection
    Collection() = delete; // Delete the default constructor to prevent instantiation without parameters
    Collection(uint8_t maxItems) : m_maxItems(maxItems) {
        m_items = new ICollectable *[maxItems]; // Allocate memory for the items array
        for (uint8_t i = 0; i < maxItems; i++) {
            m_items[i] = nullptr; // Initialize the items array to nullptr
        }
    }

    virtual ~Collection() {
        for (uint8_t i = 0; i < m_itemCount; i++) {
            delete m_items[i]; // Delete each sensor instance to free memory
        }
    }

    // --- Collection management functions --------------------
    void add(ICollectable *item) override {
        if (m_itemCount < m_maxItems) {
            m_items[m_itemCount++] = item; // Add the key to the list of keys
        }
    };
    void remove(ICollectable *item) override {
        for (int i = 0; i < m_itemCount; i++) {
            if (m_items[i] == item) {
                m_itemCount--; // Decrease the sensor count
                if (m_itemCount > 0) {
                    // Move the last sensor to the current position
                    m_items[i] = m_items[m_itemCount];
                }
                m_items[m_itemCount] = nullptr;
                break;
            }
        }
    }

    /**
     * @brief Evaluate all items in the collection.
     */
    void evaluate() override {
        for (int i = 0; i < m_itemCount; i++) {
            m_items[i]->evaluate(); // Evaluate the axes collection
        }
    }

    // --- Addressing items  ---------------------------------
    ICollectable *getItem(const uint8_t id) const override {
        if (id < m_itemCount) {
            return m_items[id]; // Return the sensor at the specified index
        }
        return nullptr; // Return nullptr if the index is out of bounds
    }

    ICollectable *getItem(const char *name) const override {
        for (uint8_t i = 0; i < m_itemCount; i++) {
            if (m_items[i] != nullptr && m_items[i]->isCurrent(name)) {
                return m_items[i];
            }
        }
        return nullptr; // Return nullptr if no matching sensor is found
    };
};
