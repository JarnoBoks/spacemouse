#pragma once

// Include the State Controller header file
#include "HIDHandlerController.hpp"

class HIDHandlerController; // Forward declaration of HIDHandlerController class

class SpaceMouseHID {
private:
    HIDHandlerController *m_controller = nullptr; // Pointer to the state machine instance
protected:
public:
    SpaceMouseHID() {
        m_controller = new HIDHandlerController(); // Initialize the state machine to the initial state
    }

    ~SpaceMouseHID() {
        delete m_controller;
    }

    inline void execute() {
        if (!m_controller) {

            return; // If the controller is not initialized, do nothing
        }
        m_controller->execute();
    }

    inline HIDHandlerController *getController() const {
        return m_controller;
    }
};