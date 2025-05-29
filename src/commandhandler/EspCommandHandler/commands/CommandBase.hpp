#pragma once
#include <Arduino.h>

class CollectionCarrier; // Forward declaration of CollectionIdentifier class

class CommandBase {
private:
    const char *name_P = nullptr; // Contains command 'name' as a PROGMEM char *
protected:
    const bool isWordEmpty(const char *str, const char *errorMsg) const; // REVIEW - Necessary to check for empty command?
    const bool convertWordNumber(const char *str, long *value) const;
    const bool convertWordFloat(const char *str, float *value) const;
    const bool convertWordBool(const char *str, bool *value) const;

    CollectionCarrier *m_CollectionCarrier = nullptr; // Pointer to the collection identifier

public:
    CommandBase() : name_P(nullptr) {}                                                                                                      // Default constructor to initialize empty command name
    CommandBase(const char *cmdName_P) : name_P(cmdName_P) {}                                                                               // Constructor to initialize command name
    CommandBase(const char *cmdName_P, CollectionCarrier *collectionCarrier) : name_P(cmdName_P), m_CollectionCarrier(collectionCarrier) {} // Constructor to initialize command name and collection identifier
    virtual ~CommandBase() = default;

    inline const bool isCommand(const char *cmdName_P) const {

        // NOTE: The strcmp_P function is used to compare the command name with the provided name.
        //       It is assumed that the second parameter ('name')  is stored in program memory (PROGMEM) to save RAM space.
        return (strcmp_P(cmdName_P, name_P) == 0); // Compare command name with the provided name
    }

    // REFACTOR - Rename to getCollectionCarrier()
    inline CollectionCarrier *getCollectionIdentifier() const { return m_CollectionCarrier; }

    virtual void execute(const char *param1, const char *param2, const uint8_t paramCount) = 0;
    virtual void stop() {}; // Stop the command execution (if applicable)
};
