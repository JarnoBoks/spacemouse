#pragma once
/**
 * Default configuration for the Joystick hardware.
 *
 * This file contains the default configuration settings for the Joystick hardware.
 * The values are used to set the default configuration for the Joystick axes as well as the sensors for the spacemouse.
 * The values can be overridden by the user in the config.h file.
 * The default values are used if there isn't a configuration in the EEPROM or if the EEPROM version is changed.
 */

#include "config.h" // Include the config.h file to check for overrides.

// --------------------- SENSOR DEFAULTS -------------------------

#ifndef PINLIST
// The pinlist should be defined in the config.h file, but if not, we define it here for failsafe purposes although the software behaviour will be undefined.
#define PINLIST {A1, A0, A3, A2, A7, A6, A9, A8}
#endif

#ifndef INVERTLIST
// The invertlist should be defined in the config.h file, but if not, we define it here for failsafe purposes although the software behaviour will be undefined.
#define INVERTLIST {0, 0, 0, 0, 0, 0, 0, 0}
#endif

// MIN and MAX values for the joystick sensors
// MIN and MAX values for the hall sensors
#ifndef MINVALS
#define MINVALS {-512, -512, -512, -512, -512, -512, -512, -512}
#endif
#ifndef MAXVALS
#define MAXVALS {512, 512, 512, 512, 512, 512, 512, 512}
#endif

// ---------------------- AXES DEFAULTS --------------------------

// Default axis configuration for spacemouse with Joystick sensors.
// ================================================================

#define NO_GATE 0

// --- Translation X axis (move knob east / west):
#ifndef DEF_SENS_TX_POS
#define DEF_SENS_TX_POS 2.0
#endif
#ifndef DEF_SENS_TX_NEG
#define DEF_SENS_TX_NEG 2.0
#endif

#ifndef DEF_GATE_TX_POS
#define DEF_GATE_TX_POS NO_GATE
#endif
#ifndef DEF_GATE_TX_NEG
#define DEF_GATE_TX_NEG NO_GATE
#endif

// --- Translation Y axis (move knob north / south):
#ifndef DEF_SENS_TY_POS
#define DEF_SENS_TY_POS 2.0
#endif
#ifndef DEF_SENS_TY_NEG
#define DEF_SENS_TY_NEG 2.0
#endif
#ifndef DEF_GATE_TY_POS
#define DEF_GATE_TY_POS NO_GATE
#endif
#ifndef DEF_GATE_TY_NEG
#define DEF_GATE_TY_NEG NO_GATE
#endif

// --- Translation Z axis (pushing knob down, pulling knob up):
#ifndef DEF_SENS_TZ_POS
#define DEF_SENS_TZ_POS 0.5
#endif
#ifndef DEF_SENS_TZ_NEG
#define DEF_SENS_TZ_NEG 2.5
#endif

#ifndef DEF_GATE_TZ_POS
#define DEF_GATE_TZ_POS NO_GATE
#endif
#ifndef DEF_GATE_TZ_NEG
#define DEF_GATE_TZ_NEG 15
#endif

// --- Ration X axis (tilt knob east / west):
#ifndef DEF_SENS_RX_POS
#define DEF_SENS_RX_POS 1.5
#endif
#ifndef DEF_SENS_RX_NEG
#define DEF_SENS_RX_NEG 1.5
#endif

#ifndef DEF_GATE_RX_POS
#define DEF_GATE_RX_POS 15
#endif
#ifndef DEF_GATE_RX_NEG
#define DEF_GATE_RX_NEG 15
#endif

// --- Ration Y axis (tilt knob north / south):
#ifndef DEF_SENS_RY_POS
#define DEF_SENS_RY_POS 1.5
#endif
#ifndef DEF_SENS_RY_NEG
#define DEF_SENS_RY_NEG 1.5
#endif

#ifndef DEF_GATE_RY_POS
#define DEF_GATE_RY_POS 15
#endif
#ifndef DEF_GATE_RY_NEG
#define DEF_GATE_RY_NEG 15
#endif

// --- Ration Z axis (turn knob clockwise / counterclockwise):
#ifndef DEF_SENS_RZ_POS
#define DEF_SENS_RZ_POS 2.0
#endif
#ifndef DEF_SENS_RZ_NEG
#define DEF_SENS_RZ_NEG 2.0
#endif

#ifndef DEF_GATE_RZ_POS
#define DEF_GATE_RZ_POS 15
#endif
#ifndef DEF_GATE_RZ_NEG
#define DEF_GATE_RZ_NEG 15
#endif

// --- Modifier Function
#ifndef DEF_MODFUNC
#define DEF_MODFUNC 3 // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif

#ifndef DEF_MF_TX_POS
#define DEF_MF_TX_POS DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_TX_NEG
#define DEF_MF_TX_NEG DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_TY_POS
#define DEF_MF_TY_POS DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_TY_NEG
#define DEF_MF_TY_NEG DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_TZ_POS
#define DEF_MF_TZ_POS 0 // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_TZ_NEG
#define DEF_MF_TZ_NEG DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_RX_POS
#define DEF_MF_RX_POS DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_RX_NEG
#define DEF_MF_RX_NEG DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_RY_POS
#define DEF_MF_RY_POS DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_RY_NEG
#define DEF_MF_RY_NEG DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_RZ_POS
#define DEF_MF_RZ_POS DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_RZ_NEG
#define DEF_MF_RZ_NEG DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif

// Setup the inversion defaults for the translation and rotation axes.
// The inversion values are used to invert the direction of the translation and rotation axes.
// 0 = no inversion, 1 = inversion

#ifndef DEF_INVERT_TX
#define DEF_INVERT_TX 1 // 0 = no inversion, 1 = inversion
#endif
#ifndef DEF_INVERT_TY
#define DEF_INVERT_TY 1 // 0 = no inversion, 1 = inversion
#endif
#ifndef DEF_INVERT_TZ
#define DEF_INVERT_TZ 1 // 0 = no inversion, 1 = inversion
#endif
#ifndef DEF_INVERT_RX
#define DEF_INVERT_RX 1 // 0 = no inversion, 1 = inversion
#endif
#ifndef DEF_INVERT_RY
#define DEF_INVERT_RY 1 // 0 = no inversion, 1 = inversion
#endif
#ifndef DEF_INVERT_RZ
#define DEF_INVERT_RZ 1 // 0 = no inversion, 1 = inversion
#endif

// --- Modifier Function
#ifndef DEF_MODFUNC
#define DEF_MODFUNC 3 // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif

#ifndef DEF_MF_TX_POS
#define DEF_MF_TX_POS DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_TX_NEG
#define DEF_MF_TX_NEG DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_TY_POS
#define DEF_MF_TY_POS DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_TY_NEG
#define DEF_MF_TY_NEG DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_TZ_POS
#define DEF_MF_TZ_POS 0 // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_TZ_NEG
#define DEF_MF_TZ_NEG DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_RX_POS
#define DEF_MF_RX_POS DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_RX_NEG
#define DEF_MF_RX_NEG DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_RY_POS
#define DEF_MF_RY_POS DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_RY_NEG
#define DEF_MF_RY_NEG DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_RZ_POS
#define DEF_MF_RZ_POS DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif
#ifndef DEF_MF_RZ_NEG
#define DEF_MF_RZ_NEG DEF_MODFUNC // 0 = linear, 1 = squared, 2 = tangent, 3 = squared tangent, 4 = cubed tangent
#endif

// Setup the inversion defaults for the translation and rotation sensors.
// The inversion values are used to invert the direction of the translation and rotation sensors.
// 0 = no inversion, 1 = inversion

#ifndef DEF_INVERT_TX
#define DEF_INVERT_TX 0
#endif
#ifndef DEF_INVERT_TY
#define DEF_INVERT_TY 1
#endif
#ifndef DEF_INVERT_TZ
#define DEF_INVERT_TZ 1
#endif
#ifndef DEF_INVERT_RX
#define DEF_INVERT_RX 0
#endif
#ifndef DEF_INVERT_RY
#define DEF_INVERT_RY 1
#endif
#ifndef DEF_INVERT_RZ
#define DEF_INVERT_RZ 1
#endif
// --------------------- KINEMATICS DEFAULTS  ----------------------------
// The kinematics defaults are used to set the default configuration for the kinematics of the spacemouse.
// The values can be overridden by the user in the config.h file.
// Switch Zoom direction with Up/Down Movement
#ifndef DEF_SWITCHYZ
#define DEF_SWITCHYZ 0 // change to 1 to switch Y and Z axis
#endif
#ifndef DEF_EXCLUSIVEMODE
#define DEF_EXCLUSIVEMODE 0 // change to 1 to enable exclusive mode
#endif

// ------------------ PREPOCESSOR DIRECTIVES USED IN THE SOFTWARE
// Change the sensor warning levels if your hardware requires. Normally this isn't necessary.

#define IDLEPOINT_LOW_WARNINGLEVEL 384  // The idlePoint should be above this level.
#define IDLEPOINT_HIGH_WARNINGLEVEL 640 // The idlePoint should be below this level.

#define DEADZONE_WARNINGLEVEL 10 // The deadzone should be below this level.
#define DEADZONE_SURPLUS 2       // The surplus that will be added to the calibrated value of the deadzone. This is used to prevent the deadzone from being too small.

#define MINIMUM_HIGH_WARNINGLEVEL -250 // The minimum value for the sensor should be below this level.
#define MAXIMUM_LOW_WARNINGLEVEL 250   // The maximum value for the sensor should be above this level.

#define WORKINGRANGE_WARNINGLEVEL 500 // The working range of the sensor should be above this level (ie. distance between MIN and MAX).
