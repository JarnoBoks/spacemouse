#pragma once

#include "HIDStateBase.h"

class HIDStateSendrotation : public HIDStateBase {
protected:
public:
    HIDStateSendrotation() = default;
    ~HIDStateSendrotation() = default;

    void apply() override final;
};