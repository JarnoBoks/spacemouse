#include "DebugOutputKeysState.hpp"
#include <key/KeyCollection.hpp>
#include <key/keys/Key.hpp>
#include <common/TextHelper.h>

void DebugOutputKeysState::update(IObservable *keyCollection) {

    // Check if the debug output is due and if the key collection is not null
    if (!isDebugOutputDue() || !keyCollection) {
        return;
    }

    // DEVNOTE - The Observable is a KeyCollection. Cast is necessary to access Collection functions.
    for (uint8_t id = 0; id < static_cast<KeyCollection *>(keyCollection)->getItemCount(); id++) {
        TextHelper::printLeadingComma(id); // Print a komma if not processing the first key

        Key *key = static_cast<KeyCollection *>(keyCollection)->getKey(id);

        Serial.print("k");
        Serial.print(key->getId()); // Print the key ID
        Serial.print(F(":"));
        Serial.print(key->getState() ? "1" : "0"); // Print the key state (pressed or released)
    }

    TextHelper::printSeparator(); // Print a separator after all keys
}
