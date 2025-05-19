#pragma once

// File contains general motion vector interface/functionality
// REFACTOR - Create real class
// REFACTOR - Move to PROGMEM
// REFACTOR - Move to common folder

constexpr const char *c_MOTIONVECTOR_DESCRIPTORS[] = {"TX", "TY", "TZ", "RX", "RY", "RZ"}; // Axis names for serial output, ordered by the MotionVector_t.
