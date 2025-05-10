#pragma once
#include "IKeyFunctionality.h"
#include "hidhandler/commands/ICommand.hpp" // For Command interface

#include <Arduino.h> // For Serial communication
// REMOVE - ENTIRE FILE IS OBSOLETE

/**
 * @brief CommandKeyFunctionality class handles the execution of HID commands on key press and release events.
 * @details This class implements the IKeyFunctionality interface and provides the functionality to execute a command when a key is pressed or released.
 *          It is used in conjunction with the KeyContext class to manage key events and their associated commands.
 */
class CommandKeyFunctionality : public IKeyFunctionality {
private:
    ICommand *m_Command = nullptr; // Pointer to the command object associated with this key functionality

public:
    CommandKeyFunctionality() = delete; // Default constructor not allowed, m_Command needs to be initialized
    CommandKeyFunctionality(ICommand *cmd) : m_Command(cmd) {}

    inline void onPress() override {
        Serial.println(F("CommandKeyFunctionality::onPress()"));
        m_Command->execute();
    };

    inline void onRelease() override {
        Serial.println(F("CommandKeyFunctionality::onRelease()"));
        m_Command->execute();
    };
};
