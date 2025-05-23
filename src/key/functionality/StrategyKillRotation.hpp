#pragma once
#include "StrategyBase.hpp"

class StrategyKillRotation : public StrategyBase {
public:
    StrategyKillRotation() = delete;
    StrategyKillRotation(Key *context_key) : StrategyBase(context_key) {} // Constructor with context

    void onPress() override {
    };
    void onRelease() override {
    };
};