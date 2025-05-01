// ButtonFactory.h
#pragma once
#include "ButtonType.h"
#include "Button.h"
#include "PhysicalButton.h"
#include "SimulatedButton.h"

class ButtonFactory {
public:
    static Button *createButton(ButtonType type, ButtonConfig *config) {
        Button *btn = nullptr;
        if (type == ButtonType::PHYSICAL)
            btn = new PhysicalButton();
        else
            btn = new SimulatedButton();
        btn->setConfig(config);
        return btn;
    }
};