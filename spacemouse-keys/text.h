#ifndef TEXT_H
#define TEXT_H

#include <Arduino.h>

/// @brief Command list for the serial interface. The commands are stored in program memory to save RAM space.

// Commands without parameters/values (1 word)
static const char CMD_IDLE[] PROGMEM = "IDLE";
static const char CMD_HELP[] PROGMEM = "HELP";
static const char CMD_SHOW[] PROGMEM = "SHOW";
static const char CMD_MINMAX[] PROGMEM = "MINMAX";

// Command with an integer value (2 words)
static const char CMD_DEBUG[] PROGMEM = "DEBUG";     // The debug level we want to set. [-1..7]
static const char CMD_MODFUNC[] PROGMEM = "MODFUNC"; // The modulation function we want to set. [0..4] ! Boundaries are inclusive, set in kinematics.h
static const char CMD_DEADZONE[] PROGMEM = "DZ";     // The value we want to set for the deadzone. [0..255] ! Boundaries are inclusive, set in config.h

// Command with a parameter and a value (3 words)
static const char CMD_INVERT[] PROGMEM = "INVERT"; // The axis we want to invert. [0..5] ! Boundaries are inclusive, set in kinematics.h
static const char CMD_SENS[] PROGMEM = "SENS";     // The axis we want to set the sensitivity for. [0..5] ! Boundaries are inclusive, set in kinematics.h

static const char RESP_HELP[] PROGMEM = "Commands: DEBUG, INVERT, MODFUNC, SENS, SHOW, HELP";

static const char Error_CommandUnkown[] PROGMEM = "Unknown command or -format.";
static const char Error[] PROGMEM = "Error: ";
static const char Error_EmptyCommand[] PROGMEM = "No command given";
static const char Error_EmptyParameter[] PROGMEM = "No parameter given for command";
static const char Error_ParameterNoNumber[] PROGMEM = "The parameter isn't a number (integer)";
static const char Error_ParameterNoFloat[] PROGMEM = "The parameter isn't a valid number (float)";
static const char Error_EmptyValue[] PROGMEM = "No value given";

// Hardware specific error messages
static const char Info_AnalogVoltage[] PROGMEM = "Setting analog reference voltage to ";

// Sensitivities:
static const char Error_InvalidAxisName[] PROGMEM = "Invalid Axisname.";

#define CF(x) ((const __FlashStringHelper *)x)

bool isWordEmpty(const char *str, const char *errorMsg);
bool convertWordNumber(const char *str, long *n, const char *errorMsg);
bool convertWordFloat(const char *str, float *const &f, const char *errorMsg);

#endif // TEXT_H
       // vim: set ts=4 sw=4 et: