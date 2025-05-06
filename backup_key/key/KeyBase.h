// KeyBase.h
#pragma once
#include "KeyConfig.h"
#include "functionality/IKeyFunctionality.h"
#include "functionality/CommandType.h" // For CommandType enum

class KeyBase {
protected:
    KeyConfig *config = nullptr;                // Pointer to the key configuration
    IKeyFunctionality *functionality = nullptr; // Pointer to the key functionality

    bool keyState = false; // Current state of the key (true = pressed, false = released)

    // Configuration settings
    int8_t id = -1;                              // ID of the key
    CommandType commandType = CommandType::NONE; // Command type for the key
public:
    KeyBase() : config(nullptr), functionality(nullptr), keyState(false), id(-1), commandType(CommandType::NONE) {}
    KeyBase(int8_t id) : config(nullptr), functionality(nullptr), keyState(false), id(id), commandType(CommandType::NONE) {}
    virtual ~KeyBase() {
        delete functionality; // Delete the functionality instance to free memory
        delete config;        // Delete the configuration instance to free memory
    }

    inline IKeyFunctionality *getFunctionality() const { return functionality; }
    inline void setFunctionality(IKeyFunctionality *func, CommandType cmd) {
        delete functionality; // Delete any previous functionality instance to free memory
        functionality = func;
        commandType = cmd;
    }

    inline CommandType getCommandType() const { return commandType; }
    inline void setCommandType(CommandType cmd) { commandType = cmd; }

    inline bool getState() const { return keyState; }
    inline void setState(bool state) { keyState = state; }

    inline virtual KeyConfig *getConfig() const { return config; }
    inline virtual void setConfig(KeyConfig *cfg) {
        delete config; // Delete any previous configuration instance to free memory
        config = cfg;
    }

    virtual void evaluate() = 0; // Pure virtual function to evaluate the key state and call the appropriate functionality
};