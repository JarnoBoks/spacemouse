#include "AxisCollection.hpp"
#include "axes\Axis.hpp" // Include the header file for the Axis class

#if 0
/**
 * @brief Setup the axis collection according to the configuration.
 * @details This method initializes the axes based on the configuration defined in config.h.
 *          It creates instances of the axes and sets their context to this AxisCollection instance.
 */
void AxisCollection::setup() {
    m_items[TRANSX] = new Axis2(TRANSX);
    m_items[TRANSY] = new Axis2(TRANSY);
    m_items[TRANSZ] = new Axis2(TRANSZ);
    m_items[ROTX] = new Axis2(ROTX);
    m_items[ROTY] = new Axis2(ROTY);
    m_items[ROTZ] = new Axis2(ROTZ);
};
#endif