// ButtonFactory.h
#pragma once
#include "ButtonType.h"
#include "ButtonBase.h"
#include "PhysicalButton.h"
#include "RotaryButton.h"
#include "config.h"

#include "functionality/KillRotationFunctionality.h"    // For KillRotationFunctionality
#include "functionality/KillTranslationFunctionality.h" // For KillRotationFunctionality
#include "functionality/CommandButtonFunctionality.h"   // For KillSwitchButtonFunctionality

class ButtonFactory {
private:
    ButtonBase *buttons[NUMKEYS + ROTARY_KEYS];
    int buttonCount = 0; // Number of buttons created
    static ButtonFactory *_instance;

public:
    ButtonFactory();
    ~ButtonFactory();

    static ButtonFactory *getInstance();

    static ButtonBase *createButton(ButtonType type, ButtonConfig *config);

    void evaluate();

    int8_t getButtonCommandsForHID(uint8_t *cmds);

    void setupButtons();
};
