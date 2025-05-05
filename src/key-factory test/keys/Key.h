// Key.h
#pragma once
#include "IKey.h"
#include "../functionality/IKeyFunctionality.h"
#include "../functionality/CommandType.h" // For CommandType enum
#include <stdint.h>
/**
 * @brief Base class for keys.
 * This class provides a common interface for different types of keys (e.g., physical, rotary).
 */
class Key : public IKey {
protected:
    IKeyFunctionality *functionality = nullptr; // Pointer to the key functionality

    bool m_keyState = false; // Current state of the key (true = pressed, false = released)

    // Configuration settings for all keys
    int8_t m_id = -1;                            // ID of the key, used for identification in f.e. printing
    CommandType commandType = CommandType::NONE; // Command type for the key        //FIXME - This is not part of the common confuguration!

public:
    // Constructor and destructor
    Key() : functionality(nullptr), m_id(-1) {}
    Key(int8_t id) : functionality(nullptr), m_id(id) {}
    virtual ~Key() {
        delete functionality; // Delete the functionality instance to free memory
    }

    // Getters and setters for key properties
    inline IKeyFunctionality *getFunctionality() const { return functionality; }
    inline void setFunctionality(IKeyFunctionality *func, CommandType cmd) {
        delete functionality; // Delete any previous functionality instance to free memory
        functionality = func;
        commandType = cmd;
    }

    inline int8_t getId() const { return m_id; } // Get the ID of the key
    inline void setId(int8_t id) { m_id = id; }  // Set the ID of the key

    inline CommandType getCommandType() const { return commandType; }
    inline void setCommandType(CommandType cmd) { commandType = cmd; }

    inline bool getState() const { return m_keyState; }
    inline void setState(bool state) { m_keyState = state; }

    // Real functionality to be implemented in derived classes
    virtual void evaluate() override = 0; // Pure virtual function to evaluate the key state and call the appropriate functionality

    virtual int8_t getHIDCommand(uint8_t *cmds); // Pure virtual function to send the HID command for the key         // TODO - This should be in the interface!
};