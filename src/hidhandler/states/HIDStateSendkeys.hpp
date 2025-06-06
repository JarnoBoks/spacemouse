#pragma once

#include "HIDStateBase.hpp"

class HIDStateSendkeys : public HIDStateBase {
protected:
public:
    HIDStateSendkeys() = default; // Default constructor
    virtual ~HIDStateSendkeys() = default;

    void apply() override final;
};