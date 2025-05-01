
// PhysicalButton.h
#pragma once
#include "Button.h"
#include "PhysicalButtonConfig.h"

class PhysicalButton : public Button {
public:
    void press() override {
        if (functionality)
            functionality->execute();
    }
    void release() override {
        // Implement release logic
    }
    void setConfig(ButtonConfig *cfg) override {
        config = dynamic_cast<PhysicalButtonConfig *>(cfg);
    }
    PhysicalButtonConfig *getConfig() const override {
        return dynamic_cast<PhysicalButtonConfig *>(config);
    }
};