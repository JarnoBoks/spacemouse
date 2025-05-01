// Button.h
#pragma once
#include "ButtonConfig.h"
#include "functionality/IButtonFunctionality.h"

class Button {
protected:
    bool buttonState = false; // Current state of the button (true = pressed, false = released)
    ButtonConfig *config;
    IButtonFunctionality *functionality;

public:
    virtual ~Button() = default;
    virtual void pressed() = 0;
    virtual void released() = 0;
    virtual void evaluate() = 0;

    inline void setFunctionality(IButtonFunctionality *func) { functionality = func; }
    inline IButtonFunctionality *getFunctionality() const { return functionality; }

    inline virtual void setConfig(ButtonConfig *cfg) { config = cfg; }
    inline virtual ButtonConfig *getConfig() const { return config; }
};