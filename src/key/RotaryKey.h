// RotaryKey.h
#pragma once
#include "KeyBase.h"

class RotaryKey : public KeyBase {
public:
    void evaluate() override {

    };

    void setConfig(KeyConfig *cfg) override {
        config = static_cast<RotaryKeyConfig *>(cfg);
    }
    RotaryKeyConfig *getConfig() const override {
        return static_cast<RotaryKeyConfig *>(config);
    }
};