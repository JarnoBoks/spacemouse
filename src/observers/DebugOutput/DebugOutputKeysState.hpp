#pragma once

#include "DebugOutput.hpp"

class DebugOutputKeysState : public DebugOutput {
public:
    DebugOutputKeysState(const bool outputNewline = true) : DebugOutput(outputNewline) {};
    virtual ~DebugOutputKeysState() {};

    void update(IObservable *keyCollection) override final;
};
