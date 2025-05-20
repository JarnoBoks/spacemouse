#pragma once

#include "KnobVectorDirectionConfig.hpp"   // For the DirectionConfig class
#include <motionvector/MotionVectorType.h> // For MotionVector_t enum

class IPrinterVisitor;

class KnobVectorConfig {
private:
    bool retrieve(const MotionVector_t motionVectorType);

public:
    KnobVectorDirectionConfig posConfig; // Object for positive direction configuration
    KnobVectorDirectionConfig negConfig; // Object for negative direction configuration
    bool inversion = false;              // Inversion flag for the knobVector // REFACTOR - MOve to kinematics

    ~KnobVectorConfig() = default; // Default destructor
    KnobVectorConfig();

    KnobVectorConfig(const MotionVector_t motionVectorType);

    KnobVectorConfig(const float psens,
                     const float nsens,
                     const uint8_t pgate,
                     const uint8_t ngate,
                     const ModFunc_t pmf,
                     const ModFunc_t nmf,
                     const bool invert);

    void persist(const MotionVector_t motionVectorType) const;

    void accept(IPrinterVisitor &visitor);
};
