
#pragma once
#include "config.h" // for ADV_HID_JIGGLE

// Include the state header files
#include "states/HIDStateData.h"  // for HIDStateData
#include "states/HIDStateBase.h"  // for HIDStateBase
#include "states/HIDStateStart.h" // for HIDStateInit

class HIDEventBufferKeys;        // forward declaration
class HIDEventBufferRotation;    // forward declaration
class HIDEventBufferTranslation; // forward declaration

/**
 * @brief This class handles the HID state machine and manages the current state.
 * @details It initializes the state machine to the initial state and sets up the context and state data.
 *          It also provides methods to execute the current state and set a new state.
 */
class HIDHandlerController {
private:
    HIDStateBase *currentState = nullptr; // Pointer to the current HID state
    HIDStateData *stateData = nullptr;    // Pointer to the state data object

    // Known interfaces for the HID event buffer
    HIDEventBufferKeys *m_HIDEventBufferKeys = nullptr;               // Pointer to the HID event buffer
    HIDEventBufferRotation *m_HIDEventBufferRotation = nullptr;       // Pointer to the HID event buffer
    HIDEventBufferTranslation *m_HIDEventBufferTranslation = nullptr; // Pointer to the HID event buffer

public:
    /**
     * @brief Constructor to initialize the HID handler controller.
     * @details This constructor initializes the state machine to the initial state and sets up the context and state data.
     *          With use of the initializer list the state is set to the 'Start' state.
     */
    HIDHandlerController() : currentState(new HIDStateStart()),
                             stateData(new HIDStateData())
    // REMOVE m_TranslatorKeys(new TranslatorKeys())
    {
        currentState->set_context(this);   // Set the context for the current state
        currentState->set_data(stateData); // Initialize the state data
    }

    /**
     * @brief Constructor to initialize the HID handler controller.
     * @details This constructor initializes the state machine to the initial state and sets up the context and state data.
     *          With use of the initializer list the state is set to the 'Start' state.
     */
    HIDHandlerController(HIDEventBufferKeys *eventBufferKeys, HIDEventBufferRotation *eventBufferRotation, HIDEventBufferTranslation *eventBufferTranslation)
        : currentState(new HIDStateStart()),
          stateData(new HIDStateData()),
          m_HIDEventBufferKeys(eventBufferKeys),
          m_HIDEventBufferRotation(eventBufferRotation),
          m_HIDEventBufferTranslation(eventBufferTranslation) {
        currentState->set_context(this);   // Set the context for the current state
        currentState->set_data(stateData); // Initialize the state data
    }

    ~HIDHandlerController() {
        delete currentState; // Clean up the current state object
        delete stateData;    // Clean up the state data object
    }

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
    inline HIDStateBase *getState() const { return currentState; }

    inline HIDEventBufferKeys *getHIDEventBufferKeys() const { return m_HIDEventBufferKeys; }                      // Get the HID event buffer for the keys
    inline HIDEventBufferRotation *getHIDEventBufferRotation() const { return m_HIDEventBufferRotation; }          // Get the HID event buffer for the rotation
    inline HIDEventBufferTranslation *getHIDEventBufferTranslation() const { return m_HIDEventBufferTranslation; } // Get the HID event buffer for the translation

    inline void setHIDEventBufferKeys(HIDEventBufferKeys *eventBufferKeys) { m_HIDEventBufferKeys = eventBufferKeys; }                                    // Set the HID event buffer for the keys
    inline void setHIDEventBufferRotation(HIDEventBufferRotation *eventBufferRotation) { m_HIDEventBufferRotation = eventBufferRotation; }                // Set the HID event buffer for the rotation
    inline void setHIDEventBufferTranslation(HIDEventBufferTranslation *eventBufferTranslation) { m_HIDEventBufferTranslation = eventBufferTranslation; } // Set the HID event buffer for the translation
};
