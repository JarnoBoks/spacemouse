#pragma once

#include <Arduino.h>

/// @brief Command list for the serial interface. The commands are stored in program memory to save RAM space.

// Commands without parameters/values (1 word)
// REMOVE static const char CMD_SHOW[] PROGMEM = "SHOW"; // Reports all configuration settings of the spacemouse.
static const char CMD_IDLE[] PROGMEM = "IDLE"; // Calibrate the idle position of the spacemouse.

// Commands with no parameters (1 word) or with an integer value (2 words)
static const char CMD_DEADZONE[] PROGMEM = "DEADZONE"; // "DEADZONE" | "DEADZONE x"    Reports or update and reports the global deadzone configuration.
//  static const char CMD_MINMAX[] PROGMEM = "MINMAX";      // TODO "MINMAX" | "MINMAX x"    Reports or updates and reports the min/max values of the spacemouse
// static const char CMD_SWITCHYZ[] PROGMEM = "SWITCHYZ";  // "SWITCHYZ" | "SWITCHYZ x"    Reports or updates the switch of YZ axes [0..1].
static const char CMD_EXCLUSIVEMODE[] PROGMEM = "EXCL"; // "EXCL" | "EXCL x" Reports or updates the Exclusive mode configuration [0..1].

// Command with an integer value (2 words)
// REMOVE static const char CMD_DEBUG[] PROGMEM = "DEBUG";     // "DEBUG x" Sets the debug level to the value x [0..11]. // TODO - set the boundaries
static const char CMD_MODFUNC[] PROGMEM = "MODFUNC"; // The modulation function we want to set. [0..4] ! Boundaries are inclusive, set in kinematics.h

// static const char PARAM_MINMAX_AUTO[] PROGMEM = "AUTO"; // The parameter for the minmax command.

// Command with a parameter and a value (3 words)
// static const char CMD_INVERT[] PROGMEM = "INVERT"; // The axis we want to invert. [0..5] ! Boundaries are inclusive, set in kinematics.h
// REMOVE static const char CMD_SENS[] PROGMEM = "SENS"; // The axis we want to set the sensitivity for. [0..5] ! Boundaries are inclusive, set in kinematics.h

// REMOVE static const char Error_CommandUnkown[] PROGMEM = "Unknown command or -format.";
// REMOVE static const char Error[] PROGMEM = "Err: ";
// REMOVE static const char Error_EmptyCommand[] PROGMEM = "No command";
// REMOVE static const char Error_EmptyParameter[] PROGMEM = "No parameter";
// REMOVE static const char Error_ParameterNoNumber[] PROGMEM = "Invalid integer";
// REMOVE static const char Error_ParameterNoFloat[] PROGMEM = "Invalid float";
// REMOVE static const char Error_EmptyValue[] PROGMEM = "No value";

// Hardware specific info messages
static const char Info_AnalogVoltage[] PROGMEM = "VREF -> ";
static const char Info_Updated[] PROGMEM = "Updated ";

// Sensitivities:
static const char Error_InvalidAxisName[] PROGMEM = "Invalid Axisname.";

#define CF(x) ((const __FlashStringHelper *)x)

// TODO - Check if we need these functions (do they save space overall?)
void helper_print(const char *text, uint8_t minwidth = 0);
void helper_print(const __FlashStringHelper *text, uint8_t minwidth = 0);
void helper_printseparator();

class TextHelper {
public:
    static void alignValue(const int value, const uint8_t width = 4);   // Align the value to the right with spaces
    static void alignedPrint(const int value, const uint8_t width = 4); // Align the value to the right with spaces
    static void printSeparator() { Serial.print(F(" | ")); }            // Print a separator between values

    static void printBooleanDescription(const bool value) {
        Serial.print(value ? F(" (ON)") : F(" (OFF)")); // Print "ON" or "OFF" based on the boolean value
    }
};
