#pragma once

#include "KnobAxisDirectionConfig.hpp"  // For the DirectionConfig class
#include <base/axis/MotionVectorType.h> // For MotionVector_t enum

class KnobAxis;

class KnobAxisConfig {
private:
    const KnobAxis *m_contextAxis = nullptr; // Pointer to the KnobAxis object that this configuration belongs to
    bool retrieve();

public:
    KnobAxisDirectionConfig posConfig; // Object for positive direction configuration
    KnobAxisDirectionConfig negConfig; // Object for negative direction configuration
    bool inversion = false;            // Inversion flag for the knobAxis // REFACTOR - Move to kinematics

    KnobAxisConfig() = delete; // Default constructor is deleted to prevent instantiation without context
    KnobAxisConfig(const KnobAxis *contextAxis);
    KnobAxisConfig(const KnobAxis *axis,
                   const float psens,
                   const float nsens,
                   const uint8_t pgate,
                   const uint8_t ngate,
                   const ModFunc_t pmf,
                   const ModFunc_t nmf,
                   const bool invert);

    ~KnobAxisConfig() = default; // Default destructor

    void persist() const;

    const bool getInvert() const { return inversion; }
};
