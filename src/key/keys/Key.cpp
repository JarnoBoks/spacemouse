#include "Key.hpp"   // Include the header file for the Key class
#include <Arduino.h> // Include Arduino library for Serial function
#if 0
int8_t Key::getHIDCommand(uint8_t *cmds) {
    if (!m_keyState) {
        return 0; // do nothing if this key is not pressed
    }

#if 0
    Serial.print(F("KeyCollection::getHIDcommands() - Key ")); // Debug output to indicate the key state
    Serial.print(i);
    Serial.print(F(" State: "));
    Serial.println(state ? "Pressed" : "Not Pressed");
    Serial.print(F("KeyFactory::getKeyCommandsForHID() - Command Type: "));
    Serial.println(static_cast<int>(commandType)); // Debug output to indicate the command type
#endif

    if (commandType == CommandType::NONE || commandType == CommandType::KILLROTATION || commandType == CommandType::KILLTRANSLATION) {
        return 0; // Skip keys without a HID command.
    }

    *cmds = static_cast<uint8_t>(commandType); // Store the command type in the result array
    return 1;
}
#endif