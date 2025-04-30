#pragma once
#include "IDebugParam.h"

class DebugParamOff : public IDebugParam {
public:
    void apply() override;
    void report() override;
};