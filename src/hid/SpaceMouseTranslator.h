#pragma once
#include "stdint.h"
#include "SpaceMouseUSBInterface.h"
/**
 * @brief Class to translate the kinematics data to the spacemouse HID interface formats
 */
class SpaceMouseTranslator {
private:
protected:
    SpaceMouseUSBInterface_ *usbInterface = nullptr; // Pointer to the USB interface
public:
    SpaceMouseTranslator(SpaceMouseUSBInterface_ *usbInterface) : usbInterface(usbInterface) {}
    virtual ~SpaceMouseTranslator() = default; // Default destructor

    virtual void execute() = 0; // Pure virtual function to be implemented by derived classes
};
