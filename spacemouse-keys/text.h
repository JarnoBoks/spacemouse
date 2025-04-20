#ifndef TEXT_H
#define TEXT_H

#include <Arduino.h>

/// @brief Command list for the serial interface. The commands are stored in program memory to save RAM space.

// Commands without parameters/values (1 word)
static const char CMD_IDLE[] PROGMEM = "IDLE";
static const char CMD_SHOW[] PROGMEM = "SHOW";
static const char CMD_MINMAX[] PROGMEM = "MINMAX";

// Command with an integer value (2 words)
static const char CMD_DEBUG[] PROGMEM = "DEBUG";     // The debug level we want to set. [-1..7]
static const char CMD_MODFUNC[] PROGMEM = "MODFUNC"; // The modulation function we want to set. [0..4] ! Boundaries are inclusive, set in kinematics.h
static const char CMD_DEADZONE[] PROGMEM = "DZ";     // The value we want to set for the deadzone. [0..255] ! Boundaries are inclusive, set in config.h

// Command with a parameter and a value (3 words)
static const char CMD_INVERT[] PROGMEM = "INVERT"; // The axis we want to invert. [0..5] ! Boundaries are inclusive, set in kinematics.h
static const char CMD_SENS[] PROGMEM = "SENS";     // The axis we want to set the sensitivity for. [0..5] ! Boundaries are inclusive, set in kinematics.h

static const char Error_CommandUnkown[] PROGMEM = "Unknown command or -format.";
static const char Error[] PROGMEM = "Err: ";
static const char Error_EmptyCommand[] PROGMEM = "No command";
static const char Error_EmptyParameter[] PROGMEM = "No parameter";
static const char Error_ParameterNoNumber[] PROGMEM = "Invalid integer";
static const char Error_ParameterNoFloat[] PROGMEM = "Invalid float";
static const char Error_EmptyValue[] PROGMEM = "No value";

// Hardware specific error messages
static const char Info_AnalogVoltage[] PROGMEM = "VREF -> ";

// Sensitivities:
static const char Error_InvalidAxisName[] PROGMEM = "Invalid Axisname.";

#define CF(x) ((const __FlashStringHelper *)x)

bool isWordEmpty(const char *str, const char *errorMsg);
bool convertWordNumber(const char *str, long *n, const char *errorMsg);
bool convertWordFloat(const char *str, float *const &f, const char *errorMsg);

void alignValue(int value);

#endif // TEXT_H
       // vim: set ts=4 sw=4 et: