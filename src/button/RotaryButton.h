// RotaryButton.h
#pragma once
#include "Button.h"
// #include "SimulatedButtonConfig.h"

class RotaryButton : public Button {
public:
    void evaluate() override {

    };

    void setConfig(ButtonConfig *cfg) override {
        config = dynamic_cast<RotaryButtonConfig *>(cfg);
    }
    RotaryButtonConfig *getConfig() const override {
        return dynamic_cast<RotaryButtonConfig *>(config);
    }
};