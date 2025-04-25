#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <Arduino.h>

class ICommand {
private:
    const char *name; // Contains command 'name' as a PROGMEM char *
protected:
    const bool isWordEmpty(const char *str, const char *errorMsg) const; // REVIEW - Necessary to check for empty command?
    const bool convertWordNumber(const char *str, long *value) const;
    const bool convertWordFloat(const char *str, float *value) const;

public:
    ICommand() : name(nullptr) {}                    // Default constructor to initialize command name
    ICommand(const char *cmdName) : name(cmdName) {} // Constructor to initialize command name
    virtual ~ICommand() = default;

    inline const bool isCommand(const char *cmdName) const {
        return strcmp_P(name, cmdName) == 0; // Compare command name with the provided name
    } // Check if the command name matches

    virtual void execute(const char *param1, const char *param2, uint8_t paramCount) = 0;
};

#endif // ICOMMAND_H