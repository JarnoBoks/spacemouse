#ifndef DP_PARAMOFF_H
#define DP_PARAMOFF_H
#include "IDebugParam.h"

class DebugParamOff : public IDebugParam {
public:
    void apply() override;
    void report() override;
};
#endif // DP_PARAMOFF_H