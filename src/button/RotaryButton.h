// RotaryButton.h
#pragma once
#include "Button.h"
// #include "SimulatedButtonConfig.h"

class RotaryButton : public Button {
public:
    void pressed() override {
        if (functionality)
            functionality->onPress();
    }
    void released() override {
        // Implement release logic
        if (functionality) {
            functionality->onRelease();
        }
    }

    void evaluate() override {};

    void setConfig(ButtonConfig *cfg) override {
        config = dynamic_cast<RotaryButtonConfig *>(cfg);
    }
    RotaryButtonConfig *getConfig() const override {
        return dynamic_cast<RotaryButtonConfig *>(config);
    }
};