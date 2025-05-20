#pragma once

#include <motionvector/MotionVectorType.h> // For MotionVector_t enum

class IPrinterVisitor;

class KinematicsMVConfig {
private:
    bool retrieve(const MotionVector_t motionVectorType);

public:
    bool inversion = false;

    ~KinematicsMVConfig() = default; // Default destructor
    KinematicsMVConfig();

    KinematicsMVConfig(const MotionVector_t motionVectorType);

    KinematicsMVConfig(const bool invert);

    void persist(const MotionVector_t motionVectorType) const;

    void accept(IPrinterVisitor &visitor);
};
