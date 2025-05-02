#pragma once

/**
 * @brief Base class to translate data to the spacemouse HID interface formats
 */
class TranslatorBase {
    // The class is used to translate the kinematics & key data to the spacemouse HID interface formats.
    // It is a base class and should be inherited by other classes that implement the execute() method.
private:
protected:
public:
    TranslatorBase() = default;          // Corrected constructor name
    virtual ~TranslatorBase() = default; // Default destructor

    virtual void execute() = 0; // Pure virtual function to be implemented by derived classes
};
