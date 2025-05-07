#include "IHIDCommand.hpp"
#include "key/keys/Key.h"
#include "hidhandler/translator/TranslatorBase.h" // Include the HID translator header for key functionality

class HIDCommandSendKey : public IHIDCommand {
private:
    Key *param_key;                   // Pointer to the key that sends the command - as paramater
    ITranslator *translator_receiver; // Pointer to the HID translator for key functionality

public:
    HIDCommandSendKey(Key *key, ITranslator *translator) : param_key(key), translator_receiver(translator) {
        // Constructor to initialize the HID command with a key and translator
    }
    ~HIDCommandSendKey() override {
        // Destructor to clean up resources
    }
    void execute() override {
        if (param_key == nullptr || translator_receiver == nullptr) {
            // If the key or translator is not set, return without doing anything
            return;
        }

        // Execute the command by sending the key to the HID interface
        // translator_receiver->sendKey(param_key); // Send the key command to the HID interface
    }
};