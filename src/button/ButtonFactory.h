// ButtonFactory.h
#pragma once
#include "ButtonType.h"
#include "Button.h"
#include "PhysicalButton.h"
#include "RotaryButton.h"
#include "config.h"

class ButtonFactory {
private:
    /// @brief The number of keys (virtual and physical) on the space mouse.
    Button *buttons[NUMKEYS + ROTARY_KEYS];
    int buttonCount;

public:
    ButtonFactory() : buttonCount(0) {}

    static Button *createButton(ButtonType type, ButtonConfig *config) {
        Button *btn = nullptr;
        if (type == ButtonType::PHYSICAL)
            btn = new PhysicalButton();
        else
            btn = new RotaryButton();
        btn->setConfig(config);
        return btn;
    }

    void processKeys() {
        for (int i = 0; i < buttonCount; i++) {
            buttons[i]->readStatus();
        }
    }
};