// KillRotationFunctionality.h
#pragma once
#include "KillSwitchButtonFunctionality.h"
#include "kinematics/kinematics.h" // For Kinematics class

class KillRotationFunctionality : public KillSwitchButtonFunctionality {
public:
    void onPress() override {
        Kinematics::getInstance()->killRotation(true);
    };
    void onRelease() override {
        Kinematics::getInstance()->killRotation(false);
    };
};