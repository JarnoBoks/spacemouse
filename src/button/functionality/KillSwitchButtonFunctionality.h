// KillSwitchButtonFunctionality.h
#pragma once
#include "IButtonFunctionality.h"

class KillSwitchButtonFunctionality : public IButtonFunctionality {
public:
    virtual void onPress() override = 0;
    virtual void onRelease() override = 0;
};