#pragma once
#include "HIDStateBase.hpp"

/**
 * @brief This class represents the starting state of the HID state machine.
 * @details For now it is an empty state, kept for future developments.
 * @note This class inherits from HIDStateBase and overrides the apply and report methods.
 */
class HIDStateStart : public HIDStateBase {
protected:
public:
    HIDStateStart() = default;
    ~HIDStateStart() = default;

    void apply() override final;
};