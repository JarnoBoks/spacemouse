// KeyFactory.h
#include "KeyFactory.h"

#include "KeyType.h"
#include "KeyBase.h"
#include "PhysicalKey.h"
#include "RotaryKey.h"
#include "config.h"

#include "functionality/KillRotationFunctionality.h"    // For KillRotationFunctionality
#include "functionality/KillTranslationFunctionality.h" // For KillRotationFunctionality
#include "functionality/CommandKeyFunctionality.h"      // For KillSwitchKeyFunctionality

KeyFactory *KeyFactory::_instance = nullptr; // Initialize the static instance to nullptr

KeyFactory::KeyFactory() : keyCount(0) {}
KeyFactory::~KeyFactory() {
    for (int i = 0; i < keyCount; i++) {
        delete keys[i]; // Delete the key instances to free memory
    }
}

KeyFactory *KeyFactory::getInstance() {
    if (_instance == nullptr) {
        _instance = new KeyFactory(); // Create a new instance of KeyFactory if it doesn't exist
    }
    return _instance;
}

KeyBase *KeyFactory::createKey(KeyType type, KeyConfig *config) {
    KeyBase *btn = nullptr;
    if (type == KeyType::PHYSICAL)
        btn = new PhysicalKey();
    else
        btn = new RotaryKey();
    btn->setConfig(config);
    return btn;
}

void KeyFactory::evaluate() {
    for (int i = 0; i < keyCount; i++) {
        keys[i]->evaluate();
    }
}

/**
 * @brief Retrieves the key commands for the HID
 * @param cmds Pointer to the array where the commands will be stored
 * @return The number of commands that have to be sent
 */
int8_t KeyFactory::getKeyCommandsForHID(uint8_t *cmds) {
    uint8_t result_idx = 0; // Index for the result array
    for (int i = 0; i < keyCount; i++) {
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
        KeyBase *btn = new PhysicalKey(i, keyPinList[i]); // Create a new key instance

        if (NUMKILLKEYS > 0 && i == KILLROT) {
            // Attach killrot functionality
            btn->setFunctionality(new KillRotationFunctionality(), CommandType::KILLROTATION);

        } else if (NUMKILLKEYS > 0 && i == KILLTRANS) {
            // Attach killtrans functionality
            btn->setFunctionality(new KillTranslationFunctionality(), CommandType::KILLTRANSLATION);

        } else {
            // Attach 'HID functionality'
            CommandType cmd = static_cast<CommandType>(btnList[idxKeylist++]); // Get the command type from the key list
            btn->setFunctionality(new CommandKeyFunctionality(cmd), cmd);      // Set the command functionality for the key
        }

        keys[keyCount++] = btn; // Add the key to the list of keys
    }

    // --- Now we check if there is a rotary encoder setup and add it to the list of keys.
    // If ROTARAY_KEYS > 0, we create rotary keys.
    for (int i = 0; i < ROTARY_KEYS; i++) {

        if (keyCount < NUMKEYS + ROTARY_KEYS) {

            // keys[keyCount++] = createKey(KeyType::ROTARY, new RotaryKeyConfig(i));
        }
    }
}
