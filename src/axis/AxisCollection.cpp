#include "AxisCollection.hpp"
#include "axis/axes/Axis.hpp" // Include the header file for the Axis class

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
