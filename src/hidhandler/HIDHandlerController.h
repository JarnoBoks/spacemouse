
#pragma once
#include "config.h" // for ADV_HID_JIGGLE

#include "states/HIDStateData.h"  // for HIDStateData
#include "states/HIDStateBase.h"  // for HIDStateBase
#include "states/HIDStateStart.h" // for HIDStateInit

class HIDHandlerController {
private:
    HIDStateBase *currentState = nullptr; // Pointer to the current HID state
    HIDStateData *stateData = nullptr;    // Pointer to the state data object - public for states to access
public:
    HIDHandlerController() : currentState(new HIDStateStart()) {
        Serial.println("HIDHandlerController::HIDHandlerController()"); // Debug output to indicate the constructor is called
        currentState->set_context(this);                                // Set the context for the current state
        stateData = new HIDStateData();                                 // Initialize the state data object that will travel along with the states.
        // currentState->set_data(new HIDStateData()); // Initialize the state data object that will travel along with the states.
    }

    // Constructor to initialize the state machine with a specific state
    HIDHandlerController(HIDStateBase *state) : currentState(state) {
        Serial.println("HIDHandlerController::HIDHandlerController(HIDStateBase *state)"); // Debug output to indicate the constructor is called
        currentState->set_context(this);                                                   // Set the context for the current state
        currentState->set_data(stateData);                                                 // Initialize the state data
    }

    ~HIDHandlerController() {
        delete currentState; // Clean up the current state
        delete stateData;    // Clean up  the state data object
    };

    /**
     * @brief Executes the current state of the HID handler controller.
     * @details This function calls the apply method of the current state,
     *          which performs the action associated with that state.
     */
    void execute() {
        Serial.println("HIDHandlerController::execute()"); // Debug output to indicate the execute method is called
        if (currentState) {
            Serial.print("now: ");
            Serial.println(stateData->now); // Debug output to indicate the state is being applied
            currentState->apply();          // Apply the current state
        }
    };

    /**
     * @brief Sets the current state of the HID handler controller.
     * @param state Pointer to the new HID state to set.
     *      This function deletes the previous state and sets the new state as the current state.
     *      It also sets the context for the new state.
     * @note The state data has been set by the previous state.
     */
    void setState(HIDStateBase *state) {
        delete currentState; // Clean up the previous state

        currentState = state;
        currentState->set_context(this);
        currentState->set_data(stateData);
    }

    /**
     * @brief Retrieves the current state of the HID handler controller.
     * @return Pointer to the current HID state.
     */
    HIDStateBase *getState() const { return currentState; }
};
