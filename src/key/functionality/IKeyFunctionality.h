
// IKeyFunctionality.h
#pragma once
class IKeyFunctionality {
public:
    virtual ~IKeyFunctionality() = default;
    virtual void onPress() = 0;   // Called when the key is pressed
    virtual void onRelease() = 0; // Called when the key is released
};