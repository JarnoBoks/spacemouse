
// IButtonFunctionality.h
#pragma once
class IButtonFunctionality {
public:
    virtual ~IButtonFunctionality() = default;
    virtual void execute() = 0;
};