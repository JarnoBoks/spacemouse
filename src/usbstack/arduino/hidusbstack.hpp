/*
This class behaves as HID Device with two endpoints for in and out

It was created by reverse-engineering a Space Navigator and relating to the HID Library by Nico Hood for reference. https://github.com/NicoHood/HID

This code is based on https://forum.arduino.cc/t/solved-unable-to-receive-hid-reports-from-computer-using-pluggableusb/596793
*/
#pragma once
// make sure that it is a supported Architecture
#ifdef ARDUINO_AVR_MICRO // ARDUINO_ARCH_AVR
#include <Arduino.h>

#include "PluggableUSB.h"
#include "HID.h"
#include <hidhandler/HIDHandlerConfig.h>

#include "usbstack/HIDReportDescriptor.h"

#define SPACEMOUSE_D_HIDREPORT(length) \
    {                                  \
        9, 0x21, 0x11, 0x01, 0, 1, 0x22, lowByte(length), highByte(length)}

typedef struct
{
    InterfaceDescriptor hid;
    HIDDescDescriptor desc;
    EndpointDescriptor in;
    EndpointDescriptor out;
} SpaceMouseHIDDescriptor;

#define USBControllerInterface pluggedInterface
#define USBControllerEndpointIn pluggedEndpoint
#define USBControllerEndpointOut (pluggedEndpoint + 1)
#define USBControllerTX USBControllerEndpointIn
#define USBControllerRX USBControllerEndpointOut

class HidUSBStack : public PluggableUSBModule {
private:
    HidUSBStack();
    static HidUSBStack *_instance;
    bool ledState;

protected:
    uint8_t endpointTypes[2];
    uint8_t protocol;
    uint8_t idle;

    int getInterface(uint8_t *interfaceNumber);
    int getDescriptor(USBSetup &setup);
    bool setup(USBSetup &setup);

public:
    static HidUSBStack *getInstance() {
        if (!_instance) {
            _instance = new HidUSBStack();
        }
        return _instance;
    }

    int write(const uint8_t *buffer, size_t size);
    int SendReport(uint8_t id, const void *data, int len);
    int readSingleByte();
    void printAllReports();
    bool updateLEDState();
    bool getLEDState();
};

// REMOVE - Replaced with instance - extern SpaceMouseUSBInterface_ SpaceMouseUSBInterface;

#endif // ARDUINO_ARCH_AVR