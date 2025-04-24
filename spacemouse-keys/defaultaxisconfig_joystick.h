#ifndef DEFAULTAXISCONFIG_JOYSTICK_H
#define DEFAULTAXISCONFIG_JOYSTICK_H

// --------------------- AXIS DEFAULTS -------------------------

// Default axis configuration for spacemouse with Joystick sensors.
// =======================================================
// This file contains the default configuration settings for the Joystick sensors.
// The values can be overridden by the user in the config.h file.
// The default values are used if there isn't a configuration in the EEPROM or if the EEPROM version is changed.

#include "config.h" // Include the config.h file to check for overrides.

#ifndef DEF_SENS_TRANSX
#define DEF_SENS_TRANSX 2.0
#endif
#ifndef DEF_SENS_TRANSY
#define DEF_SENS_TRANSY 2.0
#endif
#ifndef DEF_SENS_POSITIVE_TRANSZ
#define DEF_SENS_POSITIVE_TRANSZ 0.5
#endif
#ifndef DEF_SENS_NEGATIVE_TRANSZ
#define DEF_SENS_NEGATIVE_TRANSZ 2.5
#endif
#ifndef DEF_SENS_ROTX
#define DEF_SENS_ROTX 1.5
#endif
#ifndef DEF_SENS_ROTY
#define DEF_SENS_ROTY 1.5
#endif
#ifndef DEF_SENS_ROTZ
#define DEF_SENS_ROTZ 2.0
#endif

// The following values are used to set the gate values for the translation and rotation sensors. The gate values are used to filter out small movements of the joystick/knob.
#ifndef DEF_GATE_NEG_TRANSZ
#define DEF_GATE_NEG_TRANSZ 15
#endif
#ifndef DEF_GATE_ROTX
#define DEF_GATE_ROTX 15
#endif
#ifndef DEF_GATE_ROTY
#define DEF_GATE_ROTY 15
#endif
#ifndef DEF_GATE_ROTZ
#define DEF_GATE_ROTZ 15
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

#ifdef DEF_INVERT_TX
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

#endif // DEFAULTAXISCONFIG_JOYSTICK_H