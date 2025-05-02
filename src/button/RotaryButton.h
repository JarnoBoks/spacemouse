// RotaryButton.h
#pragma once
#include "ButtonBase.h"
// #include "SimulatedButtonConfig.h"

class RotaryButton : public ButtonBase {
public:
    void evaluate() override {

    };

    void setConfig(ButtonConfig *cfg) override {
        config = static_cast<RotaryButtonConfig *>(cfg);
    }
    RotaryButtonConfig *getConfig() const override {
        return static_cast<RotaryButtonConfig *>(config);
    }
};