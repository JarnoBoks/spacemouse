#pragma once
#include "StrategyBase.hpp"

class StrategyKillTranslation : public StrategyBase {
public:
    StrategyKillTranslation() = delete;
    StrategyKillTranslation(Key *context_key) : StrategyBase(context_key) {} // Constructor with context

    void onPress() override {
    };
    void onRelease() override {
    };
};