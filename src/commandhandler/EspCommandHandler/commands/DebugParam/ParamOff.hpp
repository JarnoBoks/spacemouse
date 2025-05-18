#pragma once
#include "IDebugParam.hpp"

/**
 * @brief Debug parameter for turning off all debugging.
 * @details This class implements the IDebugParam interface and provides methods to apply and report the debug parameter.
 * @note: This class is derived from IDebugParam and not from DebugParam
 */
class DebugParamOff : public IDebugParam {
public:
    DebugParamOff() = default;
    void apply() override;
    void report() override;
};