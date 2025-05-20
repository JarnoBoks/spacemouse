#pragma once

#include "MotionVectorDirectionConfig.hpp" // For the DirectionConfig class
#include <motionvector/MotionVectorType.h> // For MotionVector_t enum

class IPrinterVisitor; // TODO - Make Visitable

class MotionVectorConfig {
private:
    bool retrieve(const MotionVector_t motionVectorType);

public:
    MotionVectorDirectionConfig posConfig; // Object for positive direction configuration (used in the KnobVector)
    MotionVectorDirectionConfig negConfig; // Object for negative direction configuration (used in the KnobVector)
    bool inversion = false;                // Inversion flag for the KinematicsVector

    MotionVectorConfig();
    ~MotionVectorConfig() = default; // Default destructor

    MotionVectorConfig(const MotionVector_t motionVectorType);

    MotionVectorConfig(const float psens,
                       const float nsens,
                       const uint8_t pgate,
                       const uint8_t ngate,
                       const ModFunc_t pmf,
                       const ModFunc_t nmf,
                       const bool invert);

    void persist(const MotionVector_t motionVectorType) const;

    void accept(IPrinterVisitor &visitor);
};
