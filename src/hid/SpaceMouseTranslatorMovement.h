#pragma once
#include "stdint.h"
#include "SpaceMouseTranslator.h"
#include "kinematics/kinematics.h" // for the SpaceMouseKinematics class

/**
 * @brief Class to translate the kinematics data to the spacemouse HID interface formats
 */
class SpaceMouseTranslatorMovement : public SpaceMouseTranslator {
private:
    AxisType_t idx_start = AxisType_t::UNINITIALIZED; // Start index for the axis types
    AxisType_t idx_end = AxisType_t::UNINITIALIZED;   // End index for the axis types
protected:
    uint8_t message[6] = {0}; // Array to hold the message data
    bool isAllZero = true;    // Flag to indicate if all values are zero
public:
    SpaceMouseTranslatorMovement(SpaceMouseUSBInterface_ *usbInterface, AxisType_t start, AxisType_t end)
        : SpaceMouseTranslator(usbInterface), idx_start(start), idx_end(end) {}
    virtual ~SpaceMouseTranslatorMovement() = default; // Default destructor

    virtual void execute() {
        if (idx_start == AxisType_t::UNINITIALIZED || idx_end == AxisType_t::UNINITIALIZED) {
            return; // If the start or end index is uninitialized, do nothing
        }

        // Setup the message array to hold the values. The message array is used to
        // send the values to the USB interface. The array is filled either with the values
        // of the translational axes or the rotation axes.
        // Each movement is split into two bytes, one for the lower byte and one for the upper byte.
        Kinematics *Kinematics = Kinematics::getInstance();
        for (int i = idx_start; i <= idx_end; i++) {
            int16_t vel = Kinematics->getAxis(static_cast<AxisType_t>(i))->getValue();

            message[i * 2] = (byte)vel & 0xFF;     // Store the lower byte of the velocity
            message[i * 2 + 1] = (byte)(vel >> 8); // Store the upper byte of the velocity

            isAllZero &= (vel == 0); // Check if all values are zero, if one value is not zero, the flag will be false.
        }
    }

    inline bool isAllZeroValues() const {
        return isAllZero; // Return the flag indicating if all values are zero
    }
};

class SpaceMouseTranslatorTranslation : public SpaceMouseTranslatorMovement {
public:
    SpaceMouseTranslatorTranslation(SpaceMouseUSBInterface_ *usbInterface)
        : SpaceMouseTranslatorMovement(usbInterface, TRANSX, TRANSZ) {}
    virtual ~SpaceMouseTranslatorTranslation() = default; // Default destructor

    virtual void execute() override {

        SpaceMouseTranslatorMovement::execute();
        usbInterface->SendReport(1, message, 6); // send new translational values
    }
};

class SpaceMouseTranslatorRotation : public SpaceMouseTranslatorMovement {
public:
    SpaceMouseTranslatorRotation(SpaceMouseUSBInterface_ *usbInterface)
        : SpaceMouseTranslatorMovement(usbInterface, ROTX, ROTZ) {}
    virtual ~SpaceMouseTranslatorRotation() = default; // Default destructor

    virtual inline void execute() override {

        SpaceMouseTranslatorMovement::execute();
        usbInterface->SendReport(2, message, 6); // send new rotational values
    }
};