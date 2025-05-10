#pragma once

#include "IKey.h"
#include "common/ICollectable.hpp"
#include "common/Observable.hpp"
#include "hidhandler/commands/ICommand.hpp" // For Command interface

#include "key/functionality/IKeyFunctionality.h"
#include "key/functionality/CommandType.hpp" // For CommandType enum

class KeyCollection; // Forward declaration of KeyCollection class
// REMOVE class IObservable;   // Forward declaration of IObservable interface

#include <stdint.h>

/// @brief Number of observers that can be added to a key
/// @details This is a constant value that defines the maximum number of observers that can be added to the key.
/// @note This value is set to 1, as the key is only having the HID Event buffer as observer.
constexpr uint8_t c_MAX_KEY_OBSERVERS = 1;

/**
 * @brief   Base class representing a key, implementing the ICollectable interface.
 * @details This class provides a common interface for different types of keys. It provides functionality for managing key states and executing associated commands.
 * @note    The Key class is designed to be inherited by specific key types, such as PhysicalKey or RotaryKey.
 *          It provides functionality for managing key states and executing associated commands.
 * @warning The Key class should not be instantiated directly. Instead, use derived classes like PhysicalKey or RotaryKey.
 */
class Key : public IKey, public ICollectable, public Observable {
protected:
    IKeyFunctionality *m_keystrategy = nullptr;  // Pointer to the key functionality
    bool m_keyState = false;                     // Current state of the key (true = pressed, false = released)
    int8_t m_id = -1;                            // ID of the key, used for identification in f.e. printing
    CommandType commandType = CommandType::NONE; // Command type for the key        //FIXME - This is not part of the common confuguration!
    ICollection *m_Context = nullptr;            // Pointer to the context (KeyCollection) to which this key belongs

protected:
    Key(int8_t id) : Observable(c_MAX_KEY_OBSERVERS), m_id(id) {} // Constructor with Id, only to be used by derived classes
    Key(int8_t id, ICollection *collection)
        : Observable(c_MAX_KEY_OBSERVERS),
          m_id(id),
          m_Context(collection) {} // Constructor with ID and context

public:
    Key() = delete;

    virtual ~Key() {
        delete m_keystrategy; // Delete the m_keystrategy instance to free memory   // REVIEW - Deleting an externally created object is not a good idea!
    }

    // Getters and setters for key properties
    inline IKeyFunctionality *getStrategy() const { return m_keystrategy; }
    void setStrategy(IKeyFunctionality *strategy) {
        delete m_keystrategy; // Delete any previous strategy instance      // REVIEW - Deleting an externally created object is not a good idea!
        m_keystrategy = strategy;
    }

    inline int8_t getId() const { return m_id; } // Get the ID of the key
    inline void setId(int8_t id) { m_id = id; }  // Set the ID of the key

    inline ICollection *getContext() const { return m_Context; }          // Get the context of the key
    inline void setContext(ICollection *context) { m_Context = context; } // Set the context of the key

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