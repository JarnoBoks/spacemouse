#pragma once
#include "ITranslator.h"
#include "key/KeyCollection.hpp"                            // for KeyCollection
#include "hidhandler/usbinterface/SpaceMouseUSBInterface.h" // for SpaceMouseUSBInterface
#include "hidhandler/HIDHandlerConfig.h"                    // for HIDKEYDATASIZE
#include "hidhandler/commands/HIDCommandStoreKeyPress.hpp"

#include <stdint.h>

/**
 * @brief Base class to translate the key data to the spacemouse HID interface formats
 */
class TranslatorKeys : public ITranslator {
private:
    // REMOVE uint8_t *prevKeyData = nullptr; // Pointer to the previous data (not used in this class)
    boolean isDataStaged = false;

protected:
public:
    /// @brief Constructor for TranslatorKeys class
    /// @details This constructor initializes the keyData array and sets up the previous key data pointer.
    /// @param prevKeyData Pointer to the previous key data
    /// @deprecated //REMOVE
    // REMOVE TranslatorKeys(uint8_t *prevKeyData = nullptr) : prevKeyData(prevKeyData) {
    TranslatorKeys() {
        // Init or empty the keyData array
        for (int i = 0; i < HIDKEYDATASIZE; i++) {
            keyData[i] = 0;
        }
    }

    virtual ~TranslatorKeys() = default;

    uint8_t keyData[HIDKEYDATASIZE] = {0}; // Array to hold the key data to send

    void sendData() override {
        if (!isDataStaged) {
            return; // If no data has been staged, return without sending anything
        }
        SpaceMouseUSBInterface_ *usbInterface = SpaceMouseUSBInterface_::getInstance(); // Get the USB interface instance
        usbInterface->SendReport(3, keyData, HIDKEYDATASIZE);
        // REMOVE memcpy(prevKeyData, keyData, HIDKEYDATASIZE); // Copy the current key data to the previous key data
        isDataStaged = false; // Reset the staged flag to indicate that data has been sent
    }

    void stageDataToSend(ICommand *cmd) override {
        HIDCommandStoreKeyPress *KeyPressCommand = static_cast<HIDCommandStoreKeyPress *>(cmd); // ICommand parameter casted to HIDCommandStoreKeyPress
        if (KeyPressCommand == nullptr || KeyPressCommand->m_CommandInvoker == nullptr) {
            return; // If the command is not a HIDCommandStoreKeyPress, return without doing anything
        }

        // Get the raw command from the key that send the command
        uint8_t rawcmd = static_cast<uint8_t>(KeyPressCommand->m_CommandInvoker->getCommandType()); // Raw command data (one of the command types defined in CommandType enum)

        // Translate the rawcommand to the keyData array to store it for the next transmission
        keyData[(rawcmd / 8)] = (1 << (rawcmd % 8));

        // Set the staged flag to true to indicate that data has been staged for sending
        isDataStaged = true;

        // Debug output for the raw key bit numbers
#if 0 // TODO - create debugoutput for the raw key bitnumbers
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

    virtual bool hasStagedData() {
#if 0 // REMOVE
      // Check if there is something to send. If nothing is to be sent, go to the start state
        return (memcmp(keyData, prevKeyData, HIDKEYDATASIZE) != 0);
#endif
        return isDataStaged; // Return true if data has been staged for sending, false otherwise
    }
};
