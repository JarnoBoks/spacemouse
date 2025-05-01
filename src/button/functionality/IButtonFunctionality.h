
// IButtonFunctionality.h
#pragma once
class IButtonFunctionality {
public:
    virtual ~IButtonFunctionality() = default;
    virtual void onPress() = 0;   // Called when the button is pressed
    virtual void onRelease() = 0; // Called when the button is released
};