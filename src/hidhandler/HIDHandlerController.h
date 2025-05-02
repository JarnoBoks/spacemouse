
#pragma once
#include "config.h" // for ADV_HID_JIGGLE

#include "states/HIDStateData.h" // for HIDStateData
#include "states/HIDStateBase.h" // for HIDStateBase

class HIDHandlerController {
private:
    HIDStateBase *currentState = nullptr; // Pointer to the current HID state
public:
#if 0
    HIDHandlerController() : currentState(new SpaceMouseHIDStateInit()) {
        currentState->set_context(this);            // Set the context for the current state
        currentState->set_data(new HIDStateData()); // Initialize the state data object that will travel along with the states.
    } // Default constructor
#endif

    // Constructor to initialize the state machine with a specific state
    HIDHandlerController(HIDStateBase *state) : currentState(state) {
        currentState->set_context(this);            // Set the context for the current state
        currentState->set_data(new HIDStateData()); // Initialize the state data
    }

    ~HIDHandlerController() {
        if (currentState) {
            delete currentState; // Clean up the current state
            // FIXME - delete the state data object
        }
    };

    void execute() {
        if (currentState) {
            currentState->apply(); // Apply the current state
        }
    };

    void setState(HIDStateBase *state) {
        if (currentState) {
            delete currentState; // Clean up the previous state
        }
        currentState = state;
        currentState->set_context(this);
    }

    HIDStateBase *getState() const { return currentState; }
};
