#include "AxisCollection.hpp"

#if 0 // REMOVE - Implemented by the base classes Collection & Observable
/**
 * @brief Constructor for AxisCollection class.
 * @details This constructor initializes an empty axis collection.
 */
AxisCollection::AxisCollection() {
    // Constructor for AxisCollection class
    for (uint8_t i = 0; i < cHW_MAX_AXES; i++) {
        m_axes[i] = nullptr;
    }
    for (uint8_t i = 0; i < c_MAX_AXISCOLLECTION_OBSERVERS; i++) {
        m_observers[i] = nullptr;
    }
}
#endif

/**
 * @brief Setup the axis collection according to the configuration.
 * @details This method initializes the axes based on the configuration defined in config.h.
 *          It creates instances of the axes and sets their context to this AxisCollection instance.
 */
void AxisCollection::setup() {
    m_items[TRANSX] = new Axis(TRANSX);
    m_items[TRANSY] = new Axis(TRANSY);
    m_items[TRANSZ] = new Axis(TRANSZ);
    m_items[ROTX] = new Axis(ROTX);
    m_items[ROTY] = new Axis(ROTY);
    m_items[ROTZ] = new Axis(ROTZ);
};

#if 0 // REMOVE - Implemented by the base classes Collection & Observable
void AxisCollection::add(ICollectable *axis) {
    if (m_AxisCount < cHW_MAX_AXES) {
        m_axes[m_AxisCount++] = static_cast<Axis *>(axis);
    }
}

void AxisCollection::remove(ICollectable *axis) {
    for (int i = 0; i < m_AxisCount; i++) {
        if (m_axes[i] == static_cast<Axis *>(axis)) {
            m_AxisCount--; // Decrease the sensor count
            if (m_AxisCount > 0) {
                // Move the last sensor to the current position
                m_axes[i] = m_axes[m_AxisCount];
            }
            m_axes[m_AxisCount] = nullptr;
            break;
        }
    }
}

Axis *AxisCollection::getAxis(const uint8_t id) const {
    if (id < m_AxisCount) {
        return m_axes[id];
    }
    return nullptr;
}

Axis *AxisCollection::getAxis(const char *name) const {
    for (uint8_t i = 0; i < m_AxisCount; i++) {
        if (m_axes[i] != nullptr && m_axes[i]->isCurrentAxis(name)) {
            return m_axes[i];
        }
    }
    return nullptr; // Return nullptr if no matching sensor is found
};
#endif