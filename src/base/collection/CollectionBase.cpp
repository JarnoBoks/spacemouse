#include "CollectionBase.hpp"

CollectionBase::CollectionBase(uint8_t maxItems) : m_maxItems(maxItems) {
    m_items = new ICollectable *[maxItems]; // Allocate memory for the items array
    for (uint8_t i = 0; i < maxItems; i++) {
        m_items[i] = nullptr; // Initialize the items array to nullptr
    }
}

CollectionBase::~CollectionBase() {
    for (uint8_t i = 0; i < m_itemCount; i++) {
        delete m_items[i]; // Delete each sensor instance to free memory
    }
}

// --- CollectionBase management functions --------------------
void CollectionBase::add(ICollectable *item) {
    if (m_itemCount < m_maxItems) {
        m_items[m_itemCount++] = item; // Add the key to the list of keys
    }
}

void CollectionBase::remove(ICollectable *item) {
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
 * @details This function iterates through all items in the collection and calls their evaluate method.
 *          This allows each item to perform its own evaluation and update its state accordingly.
 * @note Derived classes are allowed to override this method to provide custom evaluation logic.
 */
void CollectionBase::evaluate() {
    for (int i = 0; i < m_itemCount; i++) {
        m_items[i]->evaluate(); // Evaluate the axes collection
    }
}

// --- Addressing items  ---------------------------------

/// @brief Get the item at the specified index
/// @param id The index of the item to retrieve
/// @return A pointer to the item at the specified index, or nullptr if the index is out of bounds
ICollectable *CollectionBase::getItem(const uint8_t id) const {
    if (id < m_maxItems) {
        return m_items[id]; // Return the sensor at the specified index
    }
    return nullptr; // Return nullptr if the index is out of bounds
}

/// @brief Get the item with the specified descriptor
/// @param descriptor The descriptor of the item to retrieve
/// @return A pointer to the item with the specified descriptor, or nullptr if no matching item is found
ICollectable *CollectionBase::getItem(const char *descriptor) const {
    for (uint8_t i = 0; i < m_itemCount; i++) {
        if (m_items[i] != nullptr && m_items[i]->hasDescriptor(descriptor)) {
            return m_items[i];
        }
    }
    return nullptr; // Return nullptr if no matching sensor is found
};

/// @brief Get the number of items in the collection
/// @return The number of items in the collection
uint8_t CollectionBase::getItemCount() const { return m_itemCount; }
