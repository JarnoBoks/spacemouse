#pragma once

#include "HIDStateBase.h"

class HIDStateSendkeys : public HIDStateBase {
protected:
public:
    HIDStateSendkeys(HIDStateData *data);

    virtual ~HIDStateSendkeys();

    void apply() override;
    inline void report() override {}
};