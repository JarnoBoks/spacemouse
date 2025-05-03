#pragma once

#include "HIDStateBase.h"

class HIDStateSendrotation : public HIDStateBase {
protected:
public:
    HIDStateSendrotation();
    virtual ~HIDStateSendrotation();

    void apply() override;
    inline void report() override {}
};