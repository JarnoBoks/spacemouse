#pragma once
#include <Arduino.h>

enum MotionVectorCollection_t : int8_t { MVC_ALL = -2,
                                         MVC_UNINITIALIZED = -1,
                                         TRANSLATION = 0,
                                         ROTATION = 1,
                                         MVC_LENGTH };