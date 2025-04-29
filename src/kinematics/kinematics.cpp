#include "kinematics.h"
#include "hardware/hardware.h"
#include "config.h"

// Static pointer for the Singleton instance of Kinematics
// This pointer is initialized to nullptr, indicating that the instance has not been created yet.
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

/**
 * @brief Retrieves an axis based on its type.
 * @param type The type of the axis to retrieve.
 * @return A pointer to the corresponding Axis object.
 */
Axis *Kinematics::getAxis(AxisType_t type) {
    return &axes[type];
}

/**
 * @brief Retrieves an axis based on its name.
 * @param name The name of the axis to retrieve.
 * @return A pointer to the corresponding Axis object, or nullptr if not found.
 */
Axis *Kinematics::getAxis(const char *name) {
    // TODO - Make progmem string for the axis names
    const __FlashStringHelper *axisNames[] PROGMEM = {F("TX"), F("TY"), F("TZ"), F("RX"), F("RY"), F("RZ")}; // Axis names
    for (int i = 0; i < 6; i++) {
        if (strcmp(name, (const char *)pgm_read_word(&(axisNames[i]))) == 0) {
            return &axes[i]; // Return the corresponding axis
        }
    }
    return nullptr; // Axis not found, return nullptr
}

/**
 * @brief  Constructor for the Kinematics class.
 * @details Initializes the axes with their respective configurations and hardware.
 * @note   This constructor sets up all the axes for the Spacemouse functionality.
 */
Kinematics::Kinematics() {

    // Initialize the axes with their respective configurations and hardware
    axes[TRANSX] = Axis(TRANSX); // Initialize the TX axis with the new objects
    axes[TRANSY] = Axis(TRANSY); // Initialize the TRANSY axis with the new objects
    axes[TRANSZ] = Axis(TRANSZ); // Initialize the TRANSZ axis with the new objects
    axes[ROTX] = Axis(ROTX);     // Initialize the RX axis with the new objects
    axes[ROTY] = Axis(ROTY);     // Initialize the ROTY axis with the new objects
    axes[ROTZ] = Axis(ROTZ);     // Initialize the ROTZ axis with the new objects
}

/**
 * @brief Processes the kinematics for all axes.
 * @details This function calculates the values for each axis based on the hardware input and configuration.
 */
void Kinematics::processKinematics() {
    for (int i = 0; i < AxisType_t::LENGTH; i++) {
        axes[i].calculateValue(); // Calculate the value for each axis
    }
}
