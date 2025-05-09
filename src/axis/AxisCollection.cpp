#include "AxisCollection.hpp"
#include "axes\Axis.hpp" // Include the header file for the Axis class

/**
 * @brief Setup the axis collection according to the configuration.
 * @details This method initializes the axes based on the configuration defined in config.h.
 *          It creates instances of the axes and sets their context to this AxisCollection instance.
 */
void AxisCollection::setup(ISensorsCalculator *sensorsCalculator) {
    m_items[TRANSX] = new Axis(TRANSX, sensorsCalculator);
    m_items[TRANSY] = new Axis(TRANSY, sensorsCalculator);
    m_items[TRANSZ] = new Axis(TRANSZ, sensorsCalculator);
    m_items[ROTX] = new Axis(ROTX, sensorsCalculator);
    m_items[ROTY] = new Axis(ROTY, sensorsCalculator);
    m_items[ROTZ] = new Axis(ROTZ, sensorsCalculator);
};

/**
 * @brief Get the axis at the specified index.
 * @param id Index of the axis to retrieve.
 * @return Pointer to the axis at the specified index, or nullptr if the index is out of bounds.
 * @note The base class Collection::getItem() can be used too, but it returns a pointer to the ICollectable interface.
 */
Axis *AxisCollection::getAxis(uint8_t id) const {
    return static_cast<Axis *>(getItem(id)); // Return the axis with the specified id
}

/**
 * @brief Get the axis with the specified name.
 * @param name Name of the axis to retrieve.
 * @return Pointer to the axis with the specified name, or nullptr if not found.
 * @note The base class Collection::getItem() can be used too, but it returns a pointer to the ICollectable interface.
 */
Axis *AxisCollection::getAxis(const char *name) const {
    return static_cast<Axis *>(getItem(name)); // Return the axis with the specified name
}