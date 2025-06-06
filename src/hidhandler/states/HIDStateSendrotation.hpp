#pragma once

#include "HIDStateBase.hpp"

class HIDStateSendrotation : public HIDStateBase {
protected:
public:
    HIDStateSendrotation() = default;
    ~HIDStateSendrotation() = default;

    void apply() override final;
};