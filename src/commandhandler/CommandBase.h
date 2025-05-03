#pragma once
#include <Arduino.h>

// #define CF(x) ((const __FlashStringHelper *)x)

class CommandBase {
private:
    const char *name; // Contains command 'name' as a PROGMEM char *
protected:
    const bool isWordEmpty(const char *str, const char *errorMsg) const; // REVIEW - Necessary to check for empty command?
    const bool convertWordNumber(const char *str, long *value) const;
    const bool convertWordFloat(const char *str, float *value) const;

public:
    CommandBase() : name(nullptr) {}                    // Default constructor to initialize empty command name
    CommandBase(const char *cmdName) : name(cmdName) {} // Constructor to initialize command name
    virtual ~CommandBase() = default;

    inline const bool isCommand(const char *cmdName) const {

        // NOTE: The strcmp_P function is used to compare the command name with the provided name.
        //       It is assumed that the second parameter ('name')  is stored in program memory (PROGMEM) to save RAM space.
        return (strcmp_P(cmdName, name) == 0); // Compare command name with the provided name
    }

    virtual void execute(const char *param1, const char *param2, const uint8_t paramCount) = 0;
    virtual void stop() {}; // Stop the command execution (if applicable)
};
