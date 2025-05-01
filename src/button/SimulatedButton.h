// SimulatedButton.h
#pragma once
#include "Button.h"
#include "SimulatedButtonConfig.h"

class SimulatedButton : public Button {
public:
    void press() override {
        if (functionality)
            functionality->execute();
    }
    void release() override {
        // Implement release logic
    }
    void setConfig(ButtonConfig *cfg) override {
        config = dynamic_cast<SimulatedButtonConfig *>(cfg);
    }
    SimulatedButtonConfig *getConfig() const override {
        return dynamic_cast<SimulatedButtonConfig *>(config);
    }
};