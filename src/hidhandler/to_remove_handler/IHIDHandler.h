#pragma once
#include <Arduino.h>

class IHIDHandler {
private:
protected:
public:
    IHIDHandler() = default;
    virtual ~IHIDHandler() = default;

    virtual void execute() = 0;
};
