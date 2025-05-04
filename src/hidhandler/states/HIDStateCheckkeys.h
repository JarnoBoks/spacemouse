#pragma once
#include "HIDStateBase.h"

class HIDStateCheckKeys : public HIDStateBase {
protected:
public:
    HIDStateCheckKeys();
    virtual ~HIDStateCheckKeys();

    void apply() override;
    inline void report() override {};
};