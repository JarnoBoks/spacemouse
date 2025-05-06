// DefaultKeyConfig.h
#pragma once
#include "KeyType.h"
#include "KeyConfig.h"

class DefaultKeyConfig {
    static DefaultKeyConfig *instance;
    DefaultKeyConfig() = default;

public:
    static DefaultKeyConfig *getInstance() {
        if (!instance)
            instance = new DefaultKeyConfig();
        return instance;
    }
#if 0
    KeyConfig *getDefaultConfig(KeyType type) {
        // Return default config for type
        if (type == KeyType::PHYSICAL) {
            KeyConfig *config = new PhysicalKeyConfig();
            return config;
        } else {
            KeyConfig *config = new RotaryKeyConfig();
            return config;
        }
    }
#endif
};