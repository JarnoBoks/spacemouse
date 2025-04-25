#ifndef COMMAND_DEBUG_H
#define COMMAND_DEBUG_H

#include "icommand.h"
#include "serialoutput/idebugmonitor.h"

/**
 * @brief Class to output raw sensor values for debugging purposes.
 * This class inherits from DebugMonitor and overrides the logSensorValues method to output raw sensor values.
 *
 * DEBUG_LEVEL = 1
 */
class Command_Debug : public ICommand {
private:
    DebugLevel Level = OFF;
    IDebugMonitor *debugMonitor = nullptr; // Pointer to the debug monitor instance

public:
    Command_Debug() : ICommand(ACTION_NONE), Level(OFF) {} // Constructor to initialize currentLevel
    Command_Debug(DebugLevel level) : Level(level) {}      // Constructor to initialize currentLevel

    void ExecuteCommand() override {
        // Execute the command to set the debug level
        if (Level == OFF) {
            Serial.println(F("Debugging off."));
        } else if (Level == SILENT) {
            Serial.println(F("Debugging silent."));
        } else {
            Serial.print(F("Debugging level set to: "));
            Serial.println(Level);
        }
    }
};

#endif // COMMAND_DEBUG_H