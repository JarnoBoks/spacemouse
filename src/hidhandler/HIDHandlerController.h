
#pragma once
#include "config.h" // for ADV_HID_JIGGLE

// Include the state header files
#include "states/HIDStateData.h"  // for HIDStateData
#include "states/HIDStateBase.h"  // for HIDStateBase
#include "states/HIDStateStart.h" // for HIDStateInit

// Include the translator header files
#include "translator/TranslatorKeys.h" // for TranslatorKeys

/**
 * @brief This class handles the HID state machine and manages the current state.
 * @details It initializes the state machine to the initial state and sets up the context and state data.
 *          It also provides methods to execute the current state and set a new state.
 */
class HIDHandlerController {
private:
    HIDStateBase *currentState = nullptr; // Pointer to the current HID state
    HIDStateData *stateData = nullptr;    // Pointer to the state data object

    // Known interfaces for the HID state machine
    TranslatorKeys *m_TranslatorKeys = nullptr; // Translator for key data

public:
    /**
     * @brief Constructor to initialize the HID handler controller.
     * @details This constructor initializes the state machine to the initial state and sets up the context and state data.
     *          With use of the initializer list the state is set to the 'Start' state.
     */
    HIDHandlerController() : currentState(new HIDStateStart()),
                             stateData(new HIDStateData()),
                             m_TranslatorKeys(new TranslatorKeys()) {
        currentState->set_context(this);   // Set the context for the current state
        currentState->set_data(stateData); // Initialize the state data
    }

    ~HIDHandlerController() {
        delete currentState; // Clean up the current state object
        delete stateData;    // Clean up the state data object
        delete m_TranslatorKeys;
    };

    /**
     * @brief Executes the current state of the HID handler controller.
     * @details This function calls the apply method of the current state,
     *          which performs the action associated with that state.
     */
    void execute() {
        if (currentState) {
            currentState->apply(); // Apply the current state
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

    TranslatorKeys *getTranslator() const { return m_TranslatorKeys; } // Get the translator for key data
};
