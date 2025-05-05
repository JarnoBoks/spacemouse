// KillRotationFunctionality.h
#pragma once
#include "KillSwitchFunctionality.h"
#include "kinematics/kinematics.h" // For Kinematics class

class KillRotationFunctionality : public KillSwitchFunctionality {
public:
    void onPress() override {
        Kinematics::getInstance()->killRotation(true);
    };
    void onRelease() override {
        Kinematics::getInstance()->killRotation(false);
    };
};