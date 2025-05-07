#pragma once

class IKey;

/**
 * @brief Interface for key functionality management.
 */
class IKeyFunctionality {
public:
    virtual ~IKeyFunctionality() = default;

    virtual void onPress() = 0;   // Pure virtual function to handle key press events
    virtual void onRelease() = 0; // Pure virtual function to handle key release events
};