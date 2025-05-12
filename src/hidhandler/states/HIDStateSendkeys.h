#pragma once

#include "HIDStateBase.h"

class HIDStateSendkeys : public HIDStateBase {
protected:
public:
    HIDStateSendkeys() = default; // Default constructor
    virtual ~HIDStateSendkeys() = default;

    void apply() override final;
};