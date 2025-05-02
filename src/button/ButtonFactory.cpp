// ButtonFactory.h
#include "ButtonFactory.h"

#include "ButtonType.h"
#include "ButtonBase.h"
#include "PhysicalButton.h"
#include "RotaryButton.h"
#include "config.h"

#include "functionality/KillRotationFunctionality.h"    // For KillRotationFunctionality
#include "functionality/KillTranslationFunctionality.h" // For KillRotationFunctionality
#include "functionality/CommandButtonFunctionality.h"   // For KillSwitchButtonFunctionality

ButtonFactory *ButtonFactory::_instance = nullptr; // Initialize the static instance to nullptr

ButtonFactory::ButtonFactory() : buttonCount(0) {}
ButtonFactory::~ButtonFactory() {
    for (int i = 0; i < buttonCount; i++) {
        delete buttons[i]; // Delete the button instances to free memory
    }
}

ButtonFactory *ButtonFactory::getInstance() {
    if (_instance == nullptr) {
        _instance = new ButtonFactory(); // Create a new instance of ButtonFactory if it doesn't exist
    }
    return _instance;
}

ButtonBase *ButtonFactory::createButton(ButtonType type, ButtonConfig *config) {
    ButtonBase *btn = nullptr;
    if (type == ButtonType::PHYSICAL)
        btn = new PhysicalButton();
    else
        btn = new RotaryButton();
    btn->setConfig(config);
    return btn;
}

void ButtonFactory::evaluate() {
    for (int i = 0; i < buttonCount; i++) {
        buttons[i]->evaluate();
    }
}

int8_t ButtonFactory::getButtonCommandsForHID(uint8_t *cmds) {
    uint8_t result_idx = 0; // Index for the result array
    for (int i = 0; i < buttonCount; i++) {
        bool state = buttons[i]->getState(); // Get the state of each button
        if (!state) {
            continue; // Skip buttons that are not pressed
        }

        CommandType cmd = buttons[i]->getCommandType(); // Get the command type for each button
        if (cmd == CommandType::NONE || cmd == CommandType::KILLROTATION || cmd == CommandType::KILLTRANSLATION) {
            continue; // Skip buttons without a HID command.
        }

        cmds[result_idx++] = static_cast<uint8_t>(cmd); // Store the command type in the result array
    }
    return result_idx; // Return the number of commands found
}

void ButtonFactory::setupButtons() {
    // Create buttons based on the configuration

    // Physical keys are created first
    // The number of buttons is limited by NUMKEYS.
    // The KEY_PINLIST should contain the Arduino pin numbers for the physical keys.
    int8_t keyPinList[NUMKEYS] = KEY_PINLIST; // Array to hold the key list

    // To know which functionality to configure, we need to know how many buttons of the list are used for the HID and how many are used for the kill-keys.
    int8_t btnList[NUMHIDKEYS] = BUTTONLIST; // Array to hold the button list
    uint8_t idxButtonlist = 0;
    for (int i = 0; i < NUMKEYS; i++) {
        // Set up a physical button.
        ButtonBase *btn = new PhysicalButton(i, keyPinList[i]); // Create a new button instance

        if (i == KILLROT) {
            // Attach killrot functionality
            btn->setFunctionality(new KillRotationFunctionality(), CommandType::KILLROTATION);

        } else if (i == KILLTRANS) {
            // Attach killtrans functionality
            btn->setFunctionality(new KillTranslationFunctionality(), CommandType::KILLTRANSLATION);

        } else {
            // Attach 'HID functionality'
            CommandType cmd = static_cast<CommandType>(btnList[idxButtonlist++]); // Get the command type from the button list
            btn->setFunctionality(new CommandButtonFunctionality(cmd), cmd);      // Set the command functionality for the button
        }

        buttons[buttonCount++] = btn; // Add the button to the list of buttons
    }

    for (int i = 0; i < ROTARY_KEYS; i++) {
        if (buttonCount < NUMKEYS + ROTARY_KEYS) {

            // buttons[buttonCount++] = createButton(ButtonType::ROTARY, new RotaryButtonConfig(i));
        }
    }
}
