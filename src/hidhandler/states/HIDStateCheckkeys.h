#pragma once
#include "HIDStateBase.h"

class HIDStateCheckKeys : public HIDStateBase {
protected:
public:
    HIDStateCheckKeys() = default;
    virtual ~HIDStateCheckKeys() = default;

    void apply() override final;
};