
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
#if 0
    ButtonConfig *getDefaultConfig(ButtonType type) {
        // Return default config for type
        if (type == ButtonType::PHYSICAL) {
            ButtonConfig *config = new PhysicalButtonConfig();
            return config;
        } else {
            ButtonConfig *config = new RotaryButtonConfig();
            return config;
        }
    }
#endif
};