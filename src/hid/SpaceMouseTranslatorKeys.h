#pragma once
#include "stdint.h"
#include "kinematics/kinematics.h" // for the SpaceMouseKinematics class

/**
 * @brief Class to translate the kinematics data to the spacemouse HID interface formats
 */
class SpaceMouseTranslatorMovement {
private:
    AxisType_t idx_start = AxisType_t::UNINITIALIZED; // Start index for the axis types
    AxisType_t idx_end = AxisType_t::UNINITIALIZED;   // End index for the axis types
protected:
    uint8_t message[6] = {0}; // Array to hold the message data
    bool isAllZero = true;    // Flag to indicate if all values are zero
public:
    SpaceMouseTranslatorMovement(AxisType_t start, AxisType_t end) : idx_start(start), idx_end(end) {}
    virtual ~SpaceMouseTranslatorMovement() = default; // Default destructor

    virtual void execute() {
        if (idx_start == AxisType_t::UNINITIALIZED || idx_end == AxisType_t::UNINITIALIZED) {
            return; // If the start or end index is uninitialized, do nothing
        }

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

    inline const uint8_t *getMessage() const {
        return message; // Return the message array // FIXME probably not doing what it should do.
    }

    virtual void reset() {
        isAllZero = true;
        memset(message, 0, sizeof(message));
    }
};

class SpaceMouseTranslatorTranslation : public SpaceMouseTranslatorMovement {
public:
    SpaceMouseTranslatorTranslation() : SpaceMouseTranslatorMovement(TRANSX, TRANSZ) {}
    virtual ~SpaceMouseTranslatorTranslation() = default; // Default destructor
};

class SpaceMouseTranslatorRotation : public SpaceMouseTranslatorMovement {
public:
    SpaceMouseTranslatorRotation() : SpaceMouseTranslatorMovement(ROTX, ROTZ) {}
    virtual ~SpaceMouseTranslatorRotation() = default; // Default destructor
};