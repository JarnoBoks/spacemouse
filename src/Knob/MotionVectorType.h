#pragma once
#include <Arduino.h>

///@see https://simple.wikipedia.org/wiki/Pitch,_yaw,_and_roll#/media/File:6DOF_en.jpg
enum MotionVector_t : int8_t { ALL = -2,
                               UNINITIALIZED = -1,
                               TRANSX = 0, // Left/Right
                               TRANSY = 1, // Forward/Backward
                               TRANSZ = 2, // Up/Down
                               ROTX = 3,   // Pitch
                               ROTY = 4,   // Roll
                               ROTZ = 5,   // Yaw
                               LENGTH };