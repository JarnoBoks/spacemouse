#include "KeyFactory.hpp"
#include "config.h" // For CFG_NUMBER_OF_KEYS and KEYCFG

// Include header files for key functionalities
#include <key/functionality/StrategyCommandKey.hpp>
#include "key/functionality/StrategyKillTranslation.hpp"
#include "key/functionality/StrategyKillRotation.hpp"

// Include header files for the commands that the keys can send
// REVIEW #include "hidhandler/commands/HIDCommandStoreKeyPress.hpp"

void KeyFactory::setupFunctionality(Key *key) {
    // Get the pinNumber from the config.h object for this key.
    // First get the number of keys from config.h, then populate the key_CFG array with the
    // key configuration from config.h.
    // Retrieve the key index from the key object we are working with.
    // Finally the command type is retrieved from the key_CFG array using the key index.
    constexpr uint8_t number_of_keys = CFG_NUMBER_OF_KEYS;
    constexpr uint8_t key_CFG[number_of_keys][3] = KEYCFG; // Array to hold the key configuration
    uint8_t id = key->getId();                             // Get the ID from the key object

    uint8_t rcmd = key_CFG[id][1];

    // REVIEW - What happens if the key is not in the key_CFG array?
    // REVIEW - What happens if the rcmd is not in the CommandType enum?

    // Store the command type in the key object
    key->setCommandType(static_cast<CommandType>(rcmd)); // Set the command type for the key

    // Set strategy for the key based on the command type
    StrategyBase *strategy = nullptr; // Pointer to the key functionality strategy
    if (rcmd == AX_KILLROT) {
        // STUB key->setFunctionality(new KillRotationFunctionality(), CommandType::KILLROTATION);
        strategy = new StrategyKillRotation(key); // KeyStrategy instance that wraps the command to send.
    } else if (rcmd == AX_KILLTRANS) {
        // Attach killtrans functionality
        strategy = new StrategyKillTranslation(key); // KeyStrategy instance that wraps the command to send.
    } else {
        // Attach command functionality
        strategy = new StrategyCommandKey(key); // KeyStrategy instance that wraps the command to send.
    }
    key->setStrategy(strategy); // Set the command functionality for the key
}
