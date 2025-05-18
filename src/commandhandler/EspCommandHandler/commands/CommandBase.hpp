#pragma once
#include <Arduino.h>

class CollectionCarrier; // Forward declaration of CollectionIdentifier class

class CommandBase {
private:
    const char *name = nullptr; // Contains command 'name' as a PROGMEM char *
protected:
    const bool isWordEmpty(const char *str, const char *errorMsg) const; // REVIEW - Necessary to check for empty command?
    const bool convertWordNumber(const char *str, long *value) const;
    const bool convertWordFloat(const char *str, float *value) const;

    CollectionCarrier *m_CollectionCarrier = nullptr; // Pointer to the collection identifier

public:
    CommandBase() : name(nullptr) {}                                                                                                  // Default constructor to initialize empty command name
    CommandBase(const char *cmdName) : name(cmdName) {}                                                                               // Constructor to initialize command name
    CommandBase(const char *cmdName, CollectionCarrier *collectionCarrier) : name(cmdName), m_CollectionCarrier(collectionCarrier) {} // Constructor to initialize command name and collection identifier
    virtual ~CommandBase() = default;

    inline const bool isCommand(const char *cmdName) const {

        // NOTE: The strcmp_P function is used to compare the command name with the provided name.
        //       It is assumed that the second parameter ('name')  is stored in program memory (PROGMEM) to save RAM space.
        return (strcmp_P(cmdName, name) == 0); // Compare command name with the provided name
    }

    inline CollectionCarrier *getCollectionIdentifier() const { return m_CollectionCarrier; }

    virtual void execute(const char *param1, const char *param2, const uint8_t paramCount) = 0;
    virtual void stop() {}; // Stop the command execution (if applicable)
};
