// RotaryKey.h
#pragma once
#include "KeyBase.h"

class RotaryKey : public KeyBase {
public:
    RotaryKey() : KeyBase() {}
    RotaryKey(int8_t id) : KeyBase(id) {}

    void evaluate() override {

    };

    void setConfig(KeyConfig *cfg) override {
        config = static_cast<RotaryKeyConfig *>(cfg);
    }
    RotaryKeyConfig *getConfig() const override {
        return static_cast<RotaryKeyConfig *>(config);
    }
};