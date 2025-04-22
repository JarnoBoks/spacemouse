#ifndef DEFAULTSHALL_H
#define DEFAULTSHALL_H

// Default sensorhardware configuration for Hall sensors hardware
//===============================================================
#ifndef DEADZONE
#define DEADZONE 10 // Default deadzone for the Hall Effect sensors. The value can be overiden in the config.h file.
#endif

// Default motion configuration for Hall sensors hardware.
// =======================================================
// This file contains the default configuration settings for the Hall Effect sensors.
// The values can be overiden by the user in the config.h file.
// The default values are used if there isn't a configuration in the EEPROM or if the EEPROM version is changed.

#include "config.h" // Include the config file for the hardware and the kinematics

#define NO_GATE 0

// --- Translation X axis (move knob east / west):
#ifndef DEF_SENS_TX_POS
#define DEF_SENS_TX_POS 0.8
#endif
#ifndef DEF_SENS_TX_NEG
#define DEF_SENS_TX_NEG 0.8
#endif

#ifndef DEF_GATE_TX_POS
#define DEF_GATE_TX_POS NO_GATE
#endif
#ifndef DEF_GATE_TX_NEG
#define DEF_GATE_TX_NEG NO_GATE
#endif

// --- Translation Y axis (move knob north / south):
#ifndef DEF_SENS_TY_POS
#define DEF_SENS_TY_POS 0.99
#endif
#ifndef DEF_SENS_TY_NEG
#define DEF_SENS_TY_NEG 0.99
#endif
#ifndef DEF_GATE_TY_POS
#define DEF_GATE_TY_POS NO_GATE
#endif
#ifndef DEF_GATE_TY_NEG
#define DEF_GATE_TY_NEG NO_GATE
#endif

#ifndef DEF_SENS_TZ_POS
#define DEF_SENS_TZ_POS 0.99
#endif

#ifndef DEF_GATE_TY_POS
#define DEF_GATE_TY_POS 0
#endif
#ifndef DEF_GATE_TY_NEG
#define DEF_GATE_TY_NEG 0
#endif

// --- Translation Z axis (pushing knob down, pulling knob up):
#ifndef DEF_SENS_TZ_POS
#define DEF_SENS_TZ_POS 2.5
#endif
#ifndef DEF_SENS_TZ_NEG
#define DEF_SENS_TZ_NEG 1.5
#endif

#ifndef DEF_GATE_TZ_POS
#define DEF_GATE_TZ_POS NO_GATE
#endif
#ifndef DEF_GATE_TZ_NEG
#define DEF_GATE_TZ_NEG 15
#endif

// --- Ration X axis (tilt knob east / west):
#ifndef DEF_SENS_RX_POS
#define DEF_SENS_RX_POS 1.0
#endif
#ifndef DEF_SENS_RX_NEG
#define DEF_SENS_RX_NEG 1.0
#endif

#ifndef DEF_GATE_RX_POS
#define DEF_GATE_RX_POS 15
#endif
#ifndef DEF_GATE_RX_NEG
#define DEF_GATE_RX_NEG 15
#endif

// --- Ration Y axis (tilt knob north / south):
#ifndef DEF_SENS_RY_POS
#define DEF_SENS_RY_POS 1.2
#endif
#ifndef DEF_SENS_RY_NEG
#define DEF_SENS_RY_NEG 1.2
#endif

#ifndef DEF_GATE_RY_POS
#define DEF_GATE_RY_POS 15
#endif
#ifndef DEF_GATE_RY_NEG
#define DEF_GATE_RY_NEG 15
#endif

// --- Ration Z axis (turn knob clockwise / counterclockwise):
#ifndef DEF_SENS_RZ_POS
#define DEF_SENS_RZ_POS 0.9
#endif
#ifndef DEF_SENS_RZ_NEG
#define DEF_SENS_RZ_NEG 0.9
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

// Setup the inversion defaults for the translation and rotation sensors.
// The inversion values are used to invert the direction of the translation and rotation sensors.
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

#endif // DEFAULTSHALL_H