#pragma once

class ISpaceMouseHIDState {
public:
    virtual void apply() = 0;
    virtual void report() = 0;
    virtual ~ISpaceMouseHIDState() {}
};
