// Button.h
#pragma once
#include "ButtonConfig.h"
#include "IButtonFunctionality.h"
#include "DefaultButtonConfig.h"

class Button {
protected:
    ButtonConfig *config;
    ButtonConfig *defaultConfig;
    IButtonFunctionality *functionality;

public:
    virtual ~Button() = default;
    virtual void press() = 0;
    virtual void release() = 0;

    void setFunctionality(IButtonFunctionality *func) { functionality = func; }
    IButtonFunctionality *getFunctionality() const { return functionality; }

    virtual void setConfig(ButtonConfig *cfg) { config = cfg; }
    virtual ButtonConfig *getConfig() const { return config; }

    virtual ButtonConfig *getDefaultConfig() const { return defaultConfig; }
};