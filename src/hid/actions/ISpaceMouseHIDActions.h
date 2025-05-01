#pragma once
#include <Arduino.h>

class ISpaceMouseHIDActions {
private:
protected:
public:
    ISpaceMouseHIDActions() = default;
    virtual ~ISpaceMouseHIDActions() = default;

    virtual void execute() = 0;
};
