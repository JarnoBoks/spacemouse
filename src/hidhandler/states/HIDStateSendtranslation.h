#pragma once
#include "HIDStateBase.h"

class HIDStateSendtranslation : public HIDStateBase {
protected:
public:
    HIDStateSendtranslation() = default;
    ~HIDStateSendtranslation() = default;

    void apply() override final;
};