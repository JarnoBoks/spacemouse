#pragma once

#include "IKey.h"
#include "common/ICollectable.hpp"

#include "key/functionality/IKeyFunctionality.h"
#include "key/functionality/CommandType.hpp" // For CommandType enum

#include "hidhandler/commands/ICommand.hpp" // For Command interface

class KeyCollection; // Forward declaration of KeyCollection class
class IObservable;   // Forward declaration of IObservable interface

#include <stdint.h>

/**
 * @brief Base class representing a key, implementing the ICollectable interface.
 * @details This class provides a common interface for different types of keys. It provides functionality for managing key states and executing associated commands.
 * @note The Key class is designed to be inherited by specific key types, such as PhysicalKey or RotaryKey.
 *        It provides functionality for managing key states and executing associated commands.
 */
class Key : public IKey, public ICollectable {
    // The Key class is a base class for different types of keys (e.g., physical keys, rotary keys).
    // It implements the IKey interface and provides functionality for managing key states and executing associated commands.
protected:
    IKeyFunctionality *m_keystrategy = nullptr; // Pointer to the key functionality

    bool m_keyState = false; // Current state of the key (true = pressed, false = released)

    // Configuration settings for all keys
    int8_t m_id = -1;                            // ID of the key, used for identification in f.e. printing
    CommandType commandType = CommandType::NONE; // Command type for the key        //FIXME - This is not part of the common confuguration!

    IObservable *m_context = nullptr; // Pointer to the context (KeyCollection) to which this key belongs       // FIXME - Should this be IObservable

public:
    // Constructor and destructor
    Key() = default;
    Key(int8_t id) : m_id(id) {}                                           // Constructor with ID
    Key(int8_t id, IObservable *context) : m_id(id), m_context(context) {} // Constructor with ID and context

    virtual ~Key() {
        delete m_keystrategy; // Delete the m_keystrategy instance to free memory
    }
#if 0
    // Getters and setters for key properties
    inline IKeyFunctionality *getFunctionality() const { return functionality; }
    inline void setFunctionality(IKeyFunctionality *func, CommandType cmd) {
        delete functionality; // Delete any previous functionality instance
        functionality = func;
        commandType = cmd;
    }
#endif

    // Getters and setters for key properties
    inline IKeyFunctionality *getStrategy() const { return m_keystrategy; }

    void setStrategy(IKeyFunctionality *strategy) {
        if (m_keystrategy != nullptr) {
            delete m_keystrategy; // Delete any previous functionality instance
        }
        m_keystrategy = strategy;
    }

    inline int8_t getId() const { return m_id; } // Get the ID of the key
    inline void setId(int8_t id) { m_id = id; }  // Set the ID of the key

    inline IObservable *getContext() const { return m_context; }          // Get the context of the key
    inline void setContext(IObservable *context) { m_context = context; } // Set the context of the key

    inline CommandType getCommandType() const { return commandType; }
    inline void setCommandType(CommandType cmd) { commandType = cmd; }

    inline bool getState() const { return m_keyState; }
    inline void setState(bool state) { m_keyState = state; }

    // Real functionality to be implemented in derived classes
    virtual void evaluate() override = 0; // Pure virtual function to evaluate the key state and call the appropriate functionality

    virtual int8_t getHIDCommand(uint8_t *cmds); // Pure virtual function to send the HID command for the key         // TODO - This should be in the interface!

    // NOTE: isCurrent is not used for this collection, but is part of the ICollectable interface.
    const bool isCurrent(const char *name) const override { return false; };
};