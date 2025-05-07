#include "HIDCommand.hpp"
#include "key/keys/Key.hpp"
#include "hidhandler/translator/ITranslator.h" // Include the HID translator header for key functionality

class HIDCommandSendKey : public HIDCommand {
private:
    Key *param_key;                   // Pointer to the key that sends the command - as paramater
    ITranslator *translator_receiver; // Pointer to the HID translator for key functionality

public:
    HIDCommandSendKey() = delete; // Default constructor is deleted to prevent instantiation without parameters
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
        translator_receiver->storeDataToSend(param_key); // Store the command data in the translator
    }
};