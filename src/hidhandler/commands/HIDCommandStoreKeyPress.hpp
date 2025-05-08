#pragma once
#include "HIDCommand.hpp"
#include "key/keys/Key.hpp"
#include "hidhandler/translator/ITranslator.h" // Include the HID translator header for key functionality

/**
 * @brief Command class that service the key press event.
 * @details This class is used to store the key data and send it to the translator for processing.
 * @note This class is designed to handle key press events and interact with the translator for data storage.
 */
class HIDCommandStoreKeyPress : public HIDCommand {
private:
    ITranslator *translator_receiver; // Pointer to the HID translator for key functionality

public:
    Key *m_CommandInvoker; // Pointer to the Key instance that invoked the command - command parameter and thus public

    // REVIEW HIDCommandStoreKeyPress() = delete; // Default constructor is deleted to prevent instantiation without parameters

    /// @brief Constructor to initialize the HID command with a key and translator.
    HIDCommandStoreKeyPress(Key *key, ITranslator *translator)
        : translator_receiver(translator),
          m_CommandInvoker(key) {
    }

    ///@brief Destructor to clean up resources.
    ~HIDCommandStoreKeyPress() override {
    }

    /// @brief Execute the command to store the key data.
    /// @details This function checks if the key and translator are set, and if so, it invokes the translator to execute the command.
    void execute() override {
        if (m_CommandInvoker == nullptr || translator_receiver == nullptr) {
            // If the key or translator is not set, return without doing anything
            return;
        }
        translator_receiver->stageDataToSend(this); // Store the command data in the translator
    }
};