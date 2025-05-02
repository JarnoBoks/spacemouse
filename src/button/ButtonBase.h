// Button.h
#pragma once
#include "ButtonConfig.h"
#include "functionality/IButtonFunctionality.h"
#include "functionality/CommandType.h" // For CommandType enum

class ButtonBase {
protected:
    ButtonConfig *config = nullptr;                // Pointer to the button configuration
    IButtonFunctionality *functionality = nullptr; // Pointer to the button functionality

    bool buttonState = false; // Current state of the button (true = pressed, false = released)

    // Configuration settings
    int8_t id = -1;                              // ID of the button
    CommandType commandType = CommandType::NONE; // Command type for the button
public:
    ButtonBase() : config(nullptr), functionality(nullptr), buttonState(false), id(-1), commandType(CommandType::NONE) {}
    ButtonBase(int8_t id) : config(nullptr), functionality(nullptr), buttonState(false), id(id), commandType(CommandType::NONE) {}
    virtual ~ButtonBase() {
        delete functionality; // Delete the functionality instance to free memory
        delete config;        // Delete the configuration instance to free memory
    }

    inline IButtonFunctionality *getFunctionality() const { return functionality; }
    inline void setFunctionality(IButtonFunctionality *func, CommandType cmd) {
        delete functionality; // Delete any previous functionality instance to free memory
        functionality = func;
        commandType = cmd;
    }

    inline CommandType getCommandType() const { return commandType; }
    inline void setCommandType(CommandType cmd) { commandType = cmd; }

    inline bool getState() const { return buttonState; }
    inline void setState(bool state) { buttonState = state; }

    inline virtual ButtonConfig *getConfig() const { return config; }
    inline virtual void setConfig(ButtonConfig *cfg) {
        delete config; // Delete any previous configuration instance to free memory
        config = cfg;
    }

    virtual void evaluate() = 0; // Pure virtual function to evaluate the button state and call the appropriate functionality
};