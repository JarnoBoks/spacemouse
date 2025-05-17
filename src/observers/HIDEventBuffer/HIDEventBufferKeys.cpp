#include "HIDEventBufferKeys.hpp"
#include "key/keys/Key.hpp" // For Key class

void HIDEventBufferKeys::update(IObservable *key) {

    if (!key) {
        return;
    }

    const CommandType commandType = static_cast<Key *>(key)->getCommandType();
    if (commandType == CommandType::NONE) {
        return; // If the command type is NONE, return without doing anything
    }
    const uint8_t rawcmd = static_cast<uint8_t>(commandType);

    // Translate the rawcommand to the keyData array to store it for the next transmission
    // The keyData array is used to store the key events for the HID report
    // The rawcmd is divided by 8 to get the index (ie. byte) of the command in the keyData array, and the remainder is used to set the bit in that byte
    if (static_cast<Key *>(key)->getState()) {
        // If the key is pressed, set the bit in the keyData array
        m_key_message[(rawcmd / 8)] |= (1 << (rawcmd % 8));
    } else {
        // If the key is released, clear the bit in the keyData array
        m_key_message[(rawcmd / 8)] &= ~(1 << (rawcmd % 8));
    }

    // Set the staged flag to true to indicate that data has been staged for sending
    m_isStaged = true;

    // REFACTOR - Move the debug output to a separate function / observer
    const bool DEBUG = true;
    if (DEBUG) {
        // debug the key outputs
        Serial.print(F("bitnumber: "));
        Serial.print(rawcmd);
        Serial.print(F(" -> m_key_message["));
        Serial.print((rawcmd / 8));
        Serial.print(F("] = 0b"));
        for (int b = 7; b >= 0; b--) {
            Serial.print(bitRead(m_key_message[(rawcmd / 8)], b));
        }
        Serial.print(F(" / 0x"));
        Serial.println(m_key_message[(rawcmd / 8)], HEX);
    }
}
