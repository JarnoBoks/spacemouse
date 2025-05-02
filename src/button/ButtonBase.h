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
        if (functionality != nullptr)
            delete functionality; // Delete the previous functionality instance to free memory

        functionality = func;
        commandType = cmd;
    }

    inline virtual ButtonConfig *getConfig() const { return config; }
    inline virtual void setConfig(ButtonConfig *cfg) {
        if (config != nullptr)
            delete config; // Delete the previous configuration instance to free memory
        config = cfg;
    }

    virtual void evaluate() = 0; // Pure virtual function to evaluate the button state and call the appropriate functionality
};