#pragma once

#include "HIDHandlerBase.h"     // for HIDHandlerBase
#include <hid/TranslatorKeys.h> // for TranslatorKeys
#include "config.h"             // for NUMHIDKEYS, HIDMAXBUTTONS

constexpr uint8_t KEYDATASIZE = HIDMAXBUTTONS / 8; // Size of the key data array

class HIDHandlerKeys : public HIDHandlerBase {
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
    HIDHandlerKeys(TranslatorKeys *translator)
        : HIDHandlerBase(translator),
          keyData{0},
          prevKeyData{0},
          bitNumber BUTTONLIST {};
    virtual ~HIDHandlerKeys() = default; // Default destructor

    void execute() {
        // Send the key state to the Host
        translator->execute();
        memcpy(prevKeyData, keyData, HIDMAXBUTTONS / 8); // copy actual keyData to previous keyData
    };
};