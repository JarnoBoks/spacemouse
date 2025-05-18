#pragma once

#include "DebugParam.hpp"

class DebugOutputLoopFrequency; // Forward declaration of DebugOutputLoopFrequency class

class DebugParamLoopFrequency : public DebugParam {
protected:
    DebugOutputLoopFrequency *m_AxisObserver = nullptr; // Pointer to the loop frequency observer instance
public:
    DebugParamLoopFrequency() = delete; // Constructor
    DebugParamLoopFrequency(DebugCommand *context) : DebugParam(context) {}
    virtual ~DebugParamLoopFrequency(); // Destructor to clean up the observer instance

    void apply() override;
    void report() override;
};