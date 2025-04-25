#include "kinematics.h"
#include "hardware/hardware.h"
#include "config.h"

Kinematics *Kinematics::instance = nullptr;

/**
 * @brief  Retrieves the singleton instance of the Kinematics class.
 * @return A reference to the Kinematics instance.
 */
Kinematics *Kinematics::getInstance() {
    if (!instance) {
        instance = new Kinematics();
    }
    return instance;
}

Axis *Kinematics::getAxis(AxisType_t type) {
    return &axes[type];
}

/**
 * @brief  Constructor for the Kinematics class.
 * @details Initializes the axes with their respective configurations and hardware.
 * @note   This constructor sets up all the axes for the Spacemouse functionality.
 */
Kinematics::Kinematics() {

    // Initialize the axes with their respective configurations and hardware
    axes[TX] = Axis(TX); // Initialize the TX axis with the new objects
    axes[TY] = Axis(TY); // Initialize the TY axis with the new objects
    axes[TZ] = Axis(TZ); // Initialize the TZ axis with the new objects
    axes[RX] = Axis(RX); // Initialize the RX axis with the new objects
    axes[RY] = Axis(RY); // Initialize the RY axis with the new objects
    axes[RZ] = Axis(RZ); // Initialize the RZ axis with the new objects
}

void Kinematics::processKinematics() {
    for (int i = 0; i < AxisType_t::LENGTH; i++) {
        axes[i].calculateValue(); // Calculate the value for each axis
    }
}
