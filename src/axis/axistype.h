#pragma once
#include <Arduino.h>

enum AxisType_t : int8_t { ALL = -2,
                           UNINITIALIZED = -1,
                           TRANSX = 0,
                           TRANSY,
                           TRANSZ,
                           ROTX,
                           ROTY,
                           ROTZ,
                           LENGTH };