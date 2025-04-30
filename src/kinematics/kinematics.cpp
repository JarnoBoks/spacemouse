#include "kinematics.h"
#include "hardware/hardware.h"
#include "config.h"
#include "KinematicsConfig.h"
#include "KinematicsConfig.h"

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
 * @brief  Constructor for the Kinematics class.
 * @details Initializes a Kinematics object by initializing its config and initializing the
 *          axes with their respective configurations and hardware.
 * @note   This constructor sets up all the axes for the Spacemouse functionality.
 */
Kinematics::Kinematics() {
    // Initialize the configuration for the kinematics
    // The configuration is used to set the exclusive mode and switch YZ settings.
    // The configuration is loaded from the EEPROM or set to default values if the EEPROM is empty or the version is changed.
    config = new KinematicsConfig();

    // Initialize the axes with their respective configurations and hardware
    axes[TRANSX] = Axis(TRANSX);
    axes[TRANSY] = Axis(TRANSY);
    axes[TRANSZ] = Axis(TRANSZ);
    axes[ROTX] = Axis(ROTX);
    axes[ROTY] = Axis(ROTY);
    axes[ROTZ] = Axis(ROTZ);
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
// REVIEW - This function can be removed, while we have an axis name in the axis class.
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
 * @brief Processes the kinematics for all axes.
 * @details This function calculates the values for each axis based on the hardware input and configuration.
 */
void Kinematics::processKinematics() {
    for (int i = 0; i < AxisType_t::LENGTH; i++) {
        axes[i].calculateValue(); // Calculate the value for each axis
    }

    notifyObservers(); // Notify observers of changes in the kinematics
}

// REVIEW - This should be a decorator function for the axis class, but we need to check if we can use the same function for both classes.
void Kinematics::_applyExclusiveMode() {
    if (config != nullptr && config->exclusiveMode) {
        uint16_t totalRot = abs(axes[ROTX].getValue()) + abs(axes[ROTY].getValue()) + abs(axes[ROTZ].getValue());
        uint16_t totalTrans = abs(axes[TRANSX].getValue()) + abs(axes[TRANSY].getValue()) + abs(axes[TRANSZ].getValue());

        // If the total rotation is greater than the total translation, set translation axes to 0
        // Otherwise, set rotation axes to 0
        int8_t startAxis = -1;
        int8_t endAxis = -1;
        if (totalRot > totalTrans) {
            startAxis = TRANSX;
            endAxis = TRANSZ; // Set translation axes to 0
        } else {
            startAxis = ROTX;
            endAxis = ROTZ; // Set translation axes to 0
        }

        for (int i = startAxis; i <= endAxis; i++) {
            axes[i].setValue(0); // Set translation axes to 0
        }
    }
}

// REVIEW - What is the order of the exclusive mode and switch YZ?
// REVIEW - Can we switch the entire axis at once in the array?
void Kinematics::_applySwitchYZ() {
    if (config != nullptr && config->switchYZ) {
        int16_t tmp = 0;
        tmp = axes[TRANSY].getValue();
        axes[TRANSY].setValue(axes[TRANSZ].getValue());
        axes[TRANSZ].setValue(tmp);

        tmp = axes[ROTY].getValue();
        axes[ROTY].setValue(axes[ROTZ].getValue());
        axes[ROTZ].setValue(tmp);
    }
}

// REVIEW The following functions are almost exactly the same as in hardware.cpp. Maybe move them to a common base class or use templates to avoid code duplication.
/**
 * @brief Attach an observer to the kinematics class.
 * @param observer Pointer to the observer to be attached.
 * @details This function adds the observer to the observers array and increases the observer count.
 *          If the array is full, it does not add the new observer and can be modified to handle this case.
 */
void Kinematics::attachObserver(IObserver *observer) {
    if (observerCount < MAX_KINEMATICS_OBSERVERS) {
        // insert the observer into the array, at position observerCount and increase the count after inserting.
        observers[observerCount++] = observer;
    } else {
        // TODO - Handle the case when the observer array is full. Maybe remove the oldest observer or ignore the new one?
    }
};

void Kinematics::detachObserver(IObserver *observer) {
    // remove the observer from the array by replacing it with the last observer in the array and decrease the count.
    for (int i = 0; i < observerCount; i++) {
        if (observers[i] == observer) {
            observers[i] = observers[--observerCount];
            observers[observerCount] = nullptr;
            break;
        }
    }
};

void Kinematics::notifyObservers() {
    for (int i = 0; i < observerCount; i++) {
        observers[i]->update(this); // Notify each observer
    }
};

/**
 * @brief Calculates which velocity is the main action. What is the strongest movement?
 * @return index with the biggest velocity. Returns enumAxis_t::LENGTH if all in deadzone
 * @see Ledring.cpp for usage
 */
#define VELOCITYDEADZONEFORLED 10 // Deadzone for the LED ring, if the velocity is below this value, it will not be displayed on the LED ring
const AxisType_t Kinematics::getMainAxis(Axis *axis) {
    int8_t mainVelocity = -1;
    int16_t velMax = 0;
    for (int i = 0; i < AxisType_t::LENGTH; i++) {
        // bigger than deadzone and bigger than before?
        if ((abs(axes[i].getValue()) > velMax) && (abs(axes[i].getValue()) > VELOCITYDEADZONEFORLED)) {
            velMax = abs(axes[i].getValue());
            mainVelocity = i;
        }
    }
    *axis = axes[mainVelocity]; // Set the axis to the main velocity axis   //REVIEW - Probably incorrect
    return static_cast<AxisType_t>(mainVelocity);
}