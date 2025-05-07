#pragma once
#include "stdint.h" // for uint8_t, int16_t
#include "ITranslator.h"
#include <kinematics/kinematics.h> // for the SpaceMouseKinematics class

// DEBUG:
#include <Arduino.h> // for Serial

/**
 * @brief Class to translate the kinematics data to the spacemouse HID interface formats
 * @details This class is used to translate the kinematics data to the spacemouse HID interface formats.
 *          It is a base class and should be inherited by other classes that implement the execute() method.
 * @note This class also manages the message array and checks if the values that will be sent are all zero.
 * @warning Ensure that the message array is properly filled before sending reports.
 * @see TranslatorKinematicsTranslation, TranslatorKinematicsRotation
 */
class TranslatorKinematicsBase : public ITranslator {
private:
    AxisType_t idx_start = AxisType_t::UNINITIALIZED; // Start index for the axis types
    AxisType_t idx_end = AxisType_t::UNINITIALIZED;   // End index for the axis types
protected:
    uint8_t message[6] = {0}; // Array to hold the message data
    bool isAllZero = true;    // Flag to indicate if all values are zero
public:
    TranslatorKinematicsBase() : idx_start(AxisType_t::UNINITIALIZED), idx_end(AxisType_t::UNINITIALIZED) {
        for (uint8_t i = 0; i < sizeof(message); i++) {
            message[i] = 0; // Initialize the message array to zero
        }
    } // Default constructor

    TranslatorKinematicsBase(AxisType_t start, AxisType_t end)
        : idx_start(start), idx_end(end) {}

    virtual ~TranslatorKinematicsBase() = default; // Default destructor

    virtual void execute() {

        if (idx_start == AxisType_t::UNINITIALIZED || idx_end == AxisType_t::UNINITIALIZED) {
            return; // If the start or end index is uninitialized, do nothing
        }

        Kinematics *Kinematics = Kinematics::getInstance();

        for (uint8_t i = idx_start; i <= idx_end; i++) {
            int16_t vel = Kinematics->getAxis(static_cast<AxisType_t>(i))->getValue();

            uint8_t i_msg = (i - idx_start) * 2;   // Calculate the index (zero based) in the message array for the current axis
            message[i_msg] = (byte)vel & 0xFF;     // Store the lower byte of the velocity
            message[i_msg + 1] = (byte)(vel >> 8); // Store the upper byte of the velocity

            isAllZero &= (vel == 0); // Check if all values are zero, if one value is not zero, the flag will be false.
        }
    }

    inline bool isAllZeroValues() const {
        return isAllZero; // Return the flag indicating if all values are zero
    }

    inline bool areAllAxisZero() const {
        for (uint8_t i = AxisType_t::ROTX; i < AxisType_t::LENGTH; i++) {
            int16_t vel = Kinematics::getInstance()->getAxis(static_cast<AxisType_t>(i))->getValue();
            if (vel != 0) {
                return false; // If any axis is not zero, return false
            }
        }

        return true; // All axes are zero
    }
};
