#pragma once
#include "HIDStateBase.hpp"

class HIDStateSendtranslation : public HIDStateBase {
protected:
public:
    HIDStateSendtranslation() = default;
    ~HIDStateSendtranslation() = default;

    void apply() override final;
};