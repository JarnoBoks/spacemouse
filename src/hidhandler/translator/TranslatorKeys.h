#pragma once
#include "TranslatorBase.h"

/**
 * @brief Base class to translate the kinematics data to the spacemouse HID interface formats
 */
class TranslatorKeys : public TranslatorBase {
    // The class is used to translate the kinematics & key data to the spacemouse HID interface formats.
    // It is a base class and should be inherited by other classes that implement the execute() method.
private:
protected:
public:
    TranslatorKeys() = default;
    virtual ~TranslatorKeys() = default;

    virtual void execute() = 0; // Pure virtual function to be implemented by derived classes
};
