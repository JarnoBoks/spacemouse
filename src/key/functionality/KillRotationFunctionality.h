// KillRotationFunctionality.h
#pragma once
#include "KillSwitchFunctionality.h"
#include <kinematics/Kinematics.hpp>

class KillRotationFunctionality : public KillSwitchFunctionality {
public:
    void onPress() override {
#if 0 // REFACTOR
        Kinematics::getInstance()->killRotation(true);
#endif
    };
    void onRelease() override {
#if 0
        Kinematics::getInstance()->killRotation(false);
#endif
    };
};