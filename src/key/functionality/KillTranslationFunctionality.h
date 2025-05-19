// KillTranslationFunctionality.h
#pragma once
#include "KillSwitchFunctionality.h"
#include "kinematics/kinematics.h"

class KillTranslationFunctionality : public KillSwitchFunctionality {
public:
    void onPress() override {
#if 0 // REFACTOR
        Kinematics::getInstance()->killTranslation(true); // Set the kill switch for translation axes to true
#endif
    };
    void onRelease() override {
#if 0 // REFACTOR
        Kinematics::getInstance()->killTranslation(false); // Set the kill switch for translation axes to false
#endif
    };
};