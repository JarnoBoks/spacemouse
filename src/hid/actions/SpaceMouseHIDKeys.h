#pragma once

#include "SpaceMouseHIDActionsSendBase.h" // for ISpaceMouseHIDActions
#include "../SpaceMouseTranslatorMovement.h"
#include "config.h"  // for HIDMAXBUTTONS
#include <Arduino.h> // for memcpy()

constexpr uint8_t KEYDATASIZE = HIDMAXBUTTONS / 8; // Size of the key data array

class SpaceMouseHIDKeys : public SpaceMouseHIDActionsSendBase {
    // This class is used to handle the debug parameter for axis information.
    // It inherits from ISpaceMouseHIDState and implements the apply and report methods.
    // The apply method is used to apply the changes to the axis observer.
    // The report method is used to report the current state of the axis observer.
private:
    uint8_t keyData[KEYDATASIZE] = {0};     // Array to hold the key state
    uint8_t prevKeyData[KEYDATASIZE] = {0}; // Array to hold the previous key state
    uint8_t bitNumber[NUMHIDKEYS] = BUTTONLIST;

#if 0
    // Takes the data in keys and sort them into the bits of keyData
    // Which key from keyData should belong to which byte is defined in bitNumber = BUTTONLIST see config.h
    void prepareKeyBytes(SpaceKeys *SMKeys, int debug) {
        for (int i = 0; i < KEYDATASIZE; i++) // init or empty this array
        {
            keyData[i] = 0;
        }

        for (int i = 0; i < NUMHIDKEYS; i++) {
            // check for every key if it is pressed
            if (SMKeys->GetKeyState(i) == 1) {
                // set the according bit in the data bytes
                // byte no.: bitNumber[i] / 8
                // bit no.:  bitNumber[i] modulo 8
                keyData[(bitNumber[i] / 8)] = (1 << (bitNumber[i] % 8));
                if (debug == 9) {
                    // debug the key board outputs
                    Serial.print(F("bitnumber: "));
                    Serial.print(bitNumber[i]);
                    Serial.print(F(" -> keyData["));
                    Serial.print((bitNumber[i] / 8));
                    Serial.print(F("] = 0x"));
                    Serial.println(keyData[(bitNumber[i] / 8)], HEX);
                }
            }
        }
    }
#endif

public:
    // SpaceMouse HID keys
    SpaceMouseHIDKeys(SpaceMouseUSBInterface_ *usbInterface)
        : SpaceMouseHIDActionsSendBase(usbInterface),
          keyData{0},
          prevKeyData{0},
          bitNumber BUTTONLIST {};
    virtual ~SpaceMouseHIDKeys() = default; // Default destructor

    void execute() {
        // Send the key state to the Host
        translator->execute(); // Execute the translator to get the key state
        // TODO usbInterface->SendReport(3, keyData, HIDMAXBUTTONS / 8);
        memcpy(prevKeyData, keyData, HIDMAXBUTTONS / 8); // copy actual keyData to previous keyData
    };
};