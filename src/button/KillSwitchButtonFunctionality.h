// KillSwitchButtonFunctionality.h
#pragma once
#include "IButtonFunctionality.h"

class KillSwitchButtonFunctionality : public IButtonFunctionality {
public:
    virtual void execute() override = 0;
};