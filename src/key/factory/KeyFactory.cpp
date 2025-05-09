// KeyFactory.h
#include "KeyFactory.hpp"
#include "config.h" // For CFG_NUMBER_OF_KEYS and KEYCFG

// Include header files for key functionalities
#include "key/functionality/KillRotationFunctionality.h"
#include "key/functionality/KillTranslationFunctionality.h"
#include "key/functionality/CommandKeyFunctionality.hpp"

// Include header files for the commands that the keys can send
#include "hidhandler/commands/HIDCommandStoreKeyPress.hpp"

// Include header files for the command targets for the keys
#include "hidhandler/translator/TranslatorKeys.h"

void KeyFactory::setupFunctionality(Key *key) {
    // Get the pinNumber from the config.h object for this key.
    // First get the number of keys from config.h, then populate the key_CFG array with the
    // key configuration from config.h.
    // Retrieve the key index from the key object we are working with.
    // Finally the command type is retrieved from the key_CFG array using the key index.
    constexpr uint8_t number_of_keys = CFG_NUMBER_OF_KEYS;
    constexpr uint8_t key_CFG[number_of_keys][3] = KEYCFG; // Array to hold the key configuration
    uint8_t id = key->getId();                             // Get the ID from the key object
    Serial.print(F("KeyFactory::setupFunctionality() - Key ID: "));
    Serial.println(id); // Debug output to indicate the key ID

    uint8_t rcmd = key_CFG[id][1];
    Serial.print(F("KeyFactory::setupFunctionality() - Command Type: "));
    Serial.println(rcmd); // Debug output to indicate the command type

    // TODO - What happens if the key is not in the key_CFG array?
    // TODO - What happens if the rcmd is not in the CommandType enum?

    // Store the command type in the key object
    key->setCommandType(static_cast<CommandType>(rcmd)); // Set the command type for the key

    // Set strategy for the key based on the command type
    if (rcmd == AX_KILLROT) {
        // FIXME key->setFunctionality(new KillRotationFunctionality(), CommandType::KILLROTATION);
    } else if (rcmd == AX_KILLTRANS) {
        // Attach killtrans functionality
        // FIXME key->setFunctionality(new KillTranslationFunctionality(), CommandType::KILLTRANSLATION);
    } else {
        // Setup the command object for a CommandKey. This is a command that will be sent to the HID translator.
        ICommand *HIDSK = new HIDCommandStoreKeyPress(key, nullptr);     // Create a new command that the key will send
        IKeyFunctionality *cmdFunc = new CommandKeyFunctionality(HIDSK); // KeyStrategy instance that wraps the command to send.
        key->setStrategy(cmdFunc);                                       // Set the command functionality for the key
    }
}

#if 0
KeyFactory *KeyFactory::_instance = nullptr; // Initialize the static instance to nullptr

KeyFactory::KeyFactory() : m_keyCount(0) {}
KeyFactory::~KeyFactory() {
    for (int i = 0; i < m_keyCount; i++) {
        delete keys[i]; // Delete the key instances to free memory
    }
}
#endif

#if 0
KeyFactory *KeyFactory::getInstance() {
    if (_instance == nullptr) {
        _instance = new KeyFactory(); // Create a new instance of KeyFactory if it doesn't exist
    }
    return _instance;
}
#endif

#if 0 // REMOVE - Defined as pure virtual

// Base method to create empty key instances, based on the type of key requested.
Key *KeyFactory::createKey(KeyType type, KeyConfig *config) {
    Key *btn = nullptr;
    if (type == KeyType::PHYSICAL)
        btn = new PhysicalKey();
    else
        btn = new RotaryKey(); // Create a new rotary key with an invalid ID

    btn->setConfig(config);
    return btn;
}
#endif

#if 0
void KeyFactory::evaluate() {
    for (int i = 0; i < m_keyCount; i++) {
        keys[i]->evaluate();
    }
}
#endif

#if 0
/**
 * @brief Retrieves the key commands for the HID
 * @param cmds Pointer to the array where the commands will be stored
 * @return The number of commands that have to be sent
 */
int8_t KeyFactory::getKeyCommandsForHID(uint8_t *cmds) {
    uint8_t result_idx = 0; // Index for the result array
    for (int i = 0; i < m_keyCount; i++) {
        bool state = keys[i]->getState(); // Get the state of each key
        if (!state) {
            continue; // Skip keys that are not pressed
        }

#if 0
        Serial.print(F("KeyFactory::getKeyCommandsForHID() - Key ")); // Debug output to indicate the key state
        Serial.print(i);
        Serial.print(F(" State: "));
        Serial.println(state ? "Pressed" : "Not Pressed");
#endif
        CommandType cmd = keys[i]->getCommandType(); // Get the command type for each key
#if 0
        Serial.print(F("KeyFactory::getKeyCommandsForHID() - Command Type: "));
#endif
        Serial.println(static_cast<int>(cmd)); // Debug output to indicate the command type

        if (cmd == CommandType::NONE || cmd == CommandType::KILLROTATION || cmd == CommandType::KILLTRANSLATION) {
            continue; // Skip keys without a HID command.
        }

        cmds[result_idx++] = static_cast<uint8_t>(cmd); // Store the command type in the result array
    }
    return result_idx; // Return the number of commands that have to be sent
}
#endif
#if 0
void KeyFactory::setupKeys() {
    // Create keys based on the configuration in config.h

    // ---- Physical keys are created first

    // The number of keys is limited by NUMKEYS.
    // The KEY_PINLIST should contain the Arduino pin numbers for the physical keys.
    int8_t keyPinList[NUMKEYS] = KEY_PINLIST; // Array to hold the key list

    // To know which functionality to configure, we need to know how many keys of the list are used for the HID and how many are used for the kill-keys.
    int8_t btnList[NUMHIDKEYS] = BUTTONLIST; // Array to hold the key list
    uint8_t idxKeylist = 0;
    for (int i = 0; i < NUMKEYS; i++) {
        // Set up a physical key.
        Key *key = new PhysicalKey(i, keyPinList[i]); // Create a new key instance

        if (NUMKILLKEYS > 0 && i == KILLROT) {
            // Attach killrot functionality
            key->setFunctionality(new KillRotationFunctionality(), CommandType::KILLROTATION);

        } else if (NUMKILLKEYS > 0 && i == KILLTRANS) {
            // Attach killtrans functionality
            key->setFunctionality(new KillTranslationFunctionality(), CommandType::KILLTRANSLATION);

        } else {
            // Attach 'HID functionality'
            CommandType cmd = static_cast<CommandType>(btnList[idxKeylist++]); // Get the command type from the key list
            key->setFunctionality(new CommandKeyFunctionality(cmd), cmd);      // Set the command functionality for the key
        }

        keys[m_keyCount++] = key; // Add the key to the list of keys
    }

    // --- Now we check if there is a rotary encoder and if the encoder is used as a key (ROTARY_KEYS > 0).
    // If ROTARAY_KEYS == 1, we create two rotary keys.
    if (ROTARY_KEYS == 1) {
        // The rotary keys are created as a pair, one for each direction.
        uint8_t rotaryKeyBtnListIdx[2] = {ROTARY_KEY_IDX_A, ROTARY_KEY_IDX_B}; // Array to hold the rotary key index list

        for (int i = 0; i < 2; i++) {
            // Create a new rotary key instance
            Key *key = new RotaryKey(i); // Create a new rotary key instance

            // A Rotary key can only be a command key, not a kill key. Add the command functionality to the key.
            // The command type is taken from the BUTTONLIST, which is defined in config.h.
            uint8_t idx = rotaryKeyBtnListIdx[i];                     // Get the index for the rotary key
            CommandType cmd = static_cast<CommandType>(btnList[idx]); // Get the command type from the button list
            key->setFunctionality(new CommandKeyFunctionality(cmd), cmd);

            // Now setup the functionality for the rotary key
            keys[m_keyCount++] = key; // Add the key to the list of keys
        }
    }
}
#endif