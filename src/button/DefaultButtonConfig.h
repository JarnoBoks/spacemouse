
// DefaultButtonConfig.h
#pragma once
#include "ButtonType.h"
#include "ButtonConfig.h"

class DefaultButtonConfig {
    static DefaultButtonConfig *instance;
    DefaultButtonConfig() = default;

public:
    static DefaultButtonConfig *getInstance() {
        if (!instance)
            instance = new DefaultButtonConfig();
        return instance;
    }

    ButtonConfig *getDefaultConfig(ButtonType type) {
        // Return default config for type
        if (type == ButtonType::PHYSICAL)
            return new PhysicalButtonConfig();
        else
            return new RotaryButtonConfig();
    }
};