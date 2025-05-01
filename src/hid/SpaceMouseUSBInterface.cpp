/*
This class behaves as HID Device with two endpoints for in and out

It was created by reverse-engineering a Space Navigator and relating to the HID Library by Nico Hood for reference. https://github.com/NicoHood/HID

This code is based on https://forum.arduino.cc/t/solved-unable-to-receive-hid-reports-from-computer-using-pluggableusb/596793
*/
#ifdef ARDUINO_ARCH_AVR
#include <Arduino.h>
#include "config.h"

#include "SpaceMouseUSBInterface.h"

SpaceMouseUSBInterface_::SpaceMouseUSBInterface_() : PluggableUSBModule(2, 1, endpointTypes) {
    endpointTypes[0] = EP_TYPE_INTERRUPT_IN;
    endpointTypes[1] = EP_TYPE_INTERRUPT_OUT;
    PluggableUSB().plug(this);
    ledState = false;
}

int SpaceMouseUSBInterface_::getInterface(uint8_t *interfaceNumber) {
    interfaceNumber[0] += 1;
    SpaceMouseHIDDescriptor interfaceDescriptor = {
        D_INTERFACE(USBControllerInterface, 2, USB_DEVICE_CLASS_HUMAN_INTERFACE, 0, 0),
        SPACEMOUSE_D_HIDREPORT(sizeof(SpaceMouseReportDescriptor)),
        D_ENDPOINT(USB_ENDPOINT_IN(USBControllerEndpointIn), USB_ENDPOINT_TYPE_INTERRUPT, USB_EP_SIZE, 0),
        D_ENDPOINT(USB_ENDPOINT_OUT(USBControllerEndpointOut), USB_ENDPOINT_TYPE_INTERRUPT, USB_EP_SIZE, 0),
    };
    return USB_SendControl(0, &interfaceDescriptor, sizeof(interfaceDescriptor));
}

int SpaceMouseUSBInterface_::getDescriptor(USBSetup &setup) {
    // code copied and modified from NicoHood's HID-Project
    // check if it is a HID class Descriptor request
    if (setup.bmRequestType != REQUEST_DEVICETOHOST_STANDARD_INTERFACE) {
        return 0;
    }
    if (setup.wValueH != HID_REPORT_DESCRIPTOR_TYPE) {
        return 0;
    }

    // In a HID Class Descriptor wIndex cointains the interface number
    if (setup.wIndex != pluggedInterface) {
        return 0;
    }

    protocol = HID_REPORT_PROTOCOL;

    return USB_SendControl(TRANSFER_PGM, SpaceMouseReportDescriptor, sizeof(SpaceMouseReportDescriptor));
}

bool SpaceMouseUSBInterface_::setup(USBSetup &setup) {
    // code copied from NicoHood's HID-Project
    if (pluggedInterface != setup.wIndex) {
        return false;
    }

    uint8_t request = setup.bRequest;
    uint8_t requestType = setup.bmRequestType;

    if (requestType == REQUEST_DEVICETOHOST_CLASS_INTERFACE) {
        if (request == HID_GET_REPORT) {
            // TODO: HID_GetReport();
            return true;
        }
        if (request == HID_GET_PROTOCOL) {
            // TODO: Send8(protocol);
            return true;
        }
    }

    if (requestType == REQUEST_HOSTTODEVICE_CLASS_INTERFACE) {
        if (request == HID_SET_PROTOCOL) {
            protocol = setup.wValueL;
            return true;
        }
        if (request == HID_SET_IDLE) {
            idle = setup.wValueL;
            return true;
        }
        if (request == HID_SET_REPORT) {
            // If you press "Calibrate" in the windows driver of a _SpaceNavigator_ the following setup request is sent:
            // wValue: 0x0307
            // wIndex: 0 (0x0000)
            // wLength: 2
            // Data Fragment: 0700
            // Unfortunately, we are simulating a _SpaceMouse Pro Wireless (cabled)_, because it has more than two buttons
            // With this SM pro, the windows driver is NOT sending this status report and their is no point in waiting for it...
            return true;
        }
    }

    return false;
}

int SpaceMouseUSBInterface_::write(const uint8_t *buffer, size_t size) {
    return USB_Send(USBControllerTX, buffer, size);
}

/**
 * @brief Send a HID Report
 * @param id Report Id of the data to be sent
 * @param data Pointer to the data array
 * @param len  Length of the data
 * @return Length of data sent (including 1 byte for report id)
 */
int SpaceMouseUSBInterface_::SendReport(uint8_t id, const void *data, int len) {
    auto ret = USB_Send(USBControllerTX, &id, 1);
    if (ret < 0)
        return ret;
    auto ret2 = USB_Send(USBControllerTX | TRANSFER_RELEASE, data, len);
    if (ret2 < 0)
        return ret2;
    return ret + ret2;
}

/**
 * @brief Reads a single byte from the interface, if available
 * @return Returns the byte or zero
 */
int SpaceMouseUSBInterface_::readSingleByte() {
    if (USB_Available(USBControllerRX)) {
        return USB_Recv(USBControllerRX);
    } else {
        return 0;
    }
}

/**
 * @brief Try to read some reports and print them
 */
void SpaceMouseUSBInterface_::printAllReports() {
    uint8_t numBytes = USB_Available(USBControllerRX);
    if (numBytes >= 2) {
        uint8_t data[2] = {0};
        USB_Recv(USBControllerRX, data, numBytes);
        for (int i = 0; i < numBytes; i++) {
            Serial.print(data[i], HEX);
            Serial.print(", ");
        }
        Serial.println(" ");
    } else {
        // Serial.print(".");
    }
}

/**
 * @brief Check for LED hid reports (report Id: 4). This empties the RX buffer.
 * @return Returns the led status (false = off, true = on)
 * @details This function is called in the main loop to check for LED reports from the host.
 */
bool SpaceMouseUSBInterface_::updateLEDState() {
    uint8_t numBytes = USB_Available(USBControllerRX);
    if (numBytes >= 2) {
        uint8_t data[2] = {0};
        USB_Recv(USBControllerRX, data, 2);
        if (data[0] == 4) // LED report id: 4
        {
            if (data[1] == 1) // if 1, led on!
            {
                ledState = true;
                // Serial.println("led on!");
            } else {
                ledState = false;
                // Serial.println("led off!");
            }
        }
    }
    return ledState;
}

/**
 * @brief Get the LED state, which shall be updated regularly by calling updateLEDstate()
 * @return Boolean LED state
 * @details This function returns the current state of the LED, which is updated by the updateLEDState function.
 */
bool SpaceMouseUSBInterface_::getLEDState() {
    return ledState;
}

SpaceMouseUSBInterface_ SpaceMouseUSBInterface;
#endif // ARDUINO_ARCH_AVR