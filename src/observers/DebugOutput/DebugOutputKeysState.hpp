#pragma once

#include "DebugOutput.hpp"

class DebugOutputKeysState : public DebugOutput {
public:
    DebugOutputKeysState() = default;
    virtual ~DebugOutputKeysState() {};

    void update(IObservable *keyCollection) override final;
};
