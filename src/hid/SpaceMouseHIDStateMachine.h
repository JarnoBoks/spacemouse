#pragma once

#include "states/SpaceMouseHIDStateBase.h"
#include "states/SpaceMouseHIDStateInit.h"

class SpaceMouseHIDStateMachine {
private:
    SpaceMouseHIDStateBase *currentState = nullptr; // Pointer to the current debug parameter state

public:
    SpaceMouseHIDStateMachine() : currentState(new SpaceMouseHIDStateInit()) {} // Default constructor

    SpaceMouseHIDStateMachine(SpaceMouseHIDStateBase *state) : currentState(state) {
        currentState->set_context(this); // Set the context for the current state
    }

    ~SpaceMouseHIDStateMachine() {};

    void execute() {
        if (currentState) {
            currentState->apply(); // Apply the current state
        }
    };

    void setState(SpaceMouseHIDStateBase *state) {
        if (currentState) {
            delete currentState; // Clean up the previous state
        }
        currentState = state; // Set the new state
    }

    SpaceMouseHIDStateBase *getState() const {
        currentState->report(); // Report the current state
        return currentState;    // Return the current state
    }
};
