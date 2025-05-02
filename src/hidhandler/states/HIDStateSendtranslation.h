#pragma once
#include "HIDStateBase.h"

class HIDStateSendtranslation : public HIDStateBase {
protected:
public:
    HIDStateSendtranslation(HIDStateData *data);
    virtual ~HIDStateSendtranslation();

    void apply() override;
    inline void report() override {}
};