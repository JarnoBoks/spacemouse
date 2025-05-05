#include "KeyHandler.hpp"
#include <Arduino.h> // Include Arduino library for Serial  function

/**
 * @brief Retrieves the key commands for the HID
 * @param cmds Pointer to the array where the commands will be stored
 * @return The number of commands that have to be sent
 */
int8_t KeyHandler::getHIDcommands(uint8_t *cmds) {
    uint8_t result_idx = 0;
    for (int i = 0; i < m_keyCount; i++) {
        result_idx += keys[i]->getHIDCommand(cmds + result_idx); // Get the HID command for each key
    }

    return result_idx; // Return the number of commands that have to be sent
}

void KeyHandler::evaluate() {
    for (int i = 0; i < m_keyCount; i++) {
        keys[i]->evaluate();
    }
}