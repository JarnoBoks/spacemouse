#pragma once

/**
 * @brief Base class to translate data to the spacemouse HID interface formats
 */
class ITranslator {
    // The class is used to translate the kinematics & key data to the spacemouse HID interface formats.
    // It is a base class and should be inherited by other classes that implement the execute() method.
private:
protected:
public:
    ITranslator() = default;          // Corrected constructor name
    virtual ~ITranslator() = default; // Default destructor

    virtual void execute() = 0; // Pure virtual function to be implemented by derived classes
};
