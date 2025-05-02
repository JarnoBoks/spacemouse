#pragma once

#include "HIDStateBase.h"

class HIDStateSendrotation : public HIDStateBase {
protected:
public:
    HIDStateSendrotation(HIDStateData *data);
    virtual ~HIDStateSendrotation();

    void apply() override;
    inline void report() override {}
};