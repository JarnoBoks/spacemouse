#pragma once

/**
 * @brief Interface for handling SpaceMouse HID processing state.
 */
class IHIDState {
public:
    virtual void apply() = 0;
    virtual ~IHIDState() {}
};
