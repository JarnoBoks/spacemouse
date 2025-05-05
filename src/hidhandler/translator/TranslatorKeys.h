#pragma once
#include "TranslatorBase.h"
#include "key/KeyFactory.h"
#include <hidhandler/usbinterface/SpaceMouseUSBInterface.h> // for SpaceMouseUSBInterface
#include <hidhandler/HIDHandlerConfig.h>
#include <stdint.h>

/**
 * @brief Base class to translate the key data to the spacemouse HID interface formats
 */
class TranslatorKeys : public TranslatorBase {
private:
    uint8_t *prevKeyData = nullptr; // Pointer to the previous data (not used in this class)
protected:
public:
    /**
     * @brief Constructor for TranslatorKeys class
     * @details This constructor initializes the keyData array and sets up the previous key data pointer.
     * @param prevKeyData Pointer to the previous key data
     */
    TranslatorKeys(uint8_t *prevKeyData = nullptr) : prevKeyData(prevKeyData) {
        // Init or empty the keyData array
        for (int i = 0; i < KEYDATASIZE; i++) {
            keyData[i] = 0;
        }

        uint8_t cmds[NUMHIDKEYS];
        int8_t count = KeyFactory::getInstance()->getKeyCommandsForHID(cmds); // Get the button commands for HID

        for (int8_t i = 0; i < count; i++) {
            keyData[(cmds[i] / 8)] = (1 << (cmds[i] % 8));
        }
    }

    virtual ~TranslatorKeys() = default;

    uint8_t keyData[KEYDATASIZE] = {0}; // Array to hold the key state

    virtual void prepareMessage() {
        // Init or empty the keyData array
        for (int i = 0; i < KEYDATASIZE; i++) {
            keyData[i] = 0;
        }

        uint8_t cmds[NUMHIDKEYS];
        int8_t count = KeyFactory::getInstance()->getKeyCommandsForHID(cmds); // Get the button commands for HID

        for (int8_t i = 0; i < count; i++) {
            keyData[(cmds[i] / 8)] = (1 << (cmds[i] % 8));
#if 0
        if (debug == 9) {
            // debug the key board outputs
            Serial.print(F("bitnumber: "));
            Serial.print(bitNumber[i]);
            Serial.print(F(" -> keyData["));
            Serial.print((bitNumber[i] / 8));
            Serial.print(F("] = 0x"));
            Serial.println(keyData[(bitNumber[i] / 8)], HEX);
        }
#endif
        }
    }

    virtual void execute() {
        SpaceMouseUSBInterface_ *usbInterface = SpaceMouseUSBInterface_::getInstance(); // Get the USB interface instance
        usbInterface->SendReport(3, keyData, KEYDATASIZE);
        memcpy(prevKeyData, keyData, KEYDATASIZE); // Copy the current key data to the previous key data
    }

    virtual bool isAnythingChanged() {
        // Check if there is something to send. If nothing is to be sent, go to the start state
        return (memcmp(keyData, prevKeyData, KEYDATASIZE) != 0);
    }
};
