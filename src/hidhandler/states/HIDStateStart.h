#pragma once
#include "HIDStateBase.h"

/**
 * @brief This class represents the starting state of the HID state machine.
 * @details It is responsible for checking the conditions to transition to the next state.
 *          It evaluates the current state of the system and decides whether to send data or check for key presses.
 * @note This class inherits from HIDStateBase and overrides the apply and report methods.
 */
class HIDStateStart : public HIDStateBase {
protected:
public:
    HIDStateStart();
    virtual ~HIDStateStart();

    void apply() override;
    inline void report() override {};
};