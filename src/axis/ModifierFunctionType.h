#pragma once
#include <Arduino.h>

enum ModFunc_t : int8_t {
    mfUNINITIALIZED = -1,
    mfLINEAR = 0,
    mfSQUARED = 1,
    mfTANGENT = 2,
    mfSQUARDED_TANGENT = 3,
    mfCUBED_TANGENT = 4
};
