// KillTranslationFunctionality.h
#pragma once
#include "KillSwitchFunctionality.h"
#include "kinematics/kinematics.h"

class KillTranslationFunctionality : public KillSwitchFunctionality {
public:
    void onPress() override {
        Kinematics::getInstance()->killTranslation(true); // Set the kill switch for translation axes to true
    };
    void onRelease() override {
        Kinematics::getInstance()->killTranslation(false); // Set the kill switch for translation axes to false
    };
};