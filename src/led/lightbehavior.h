#ifndef LIGHTBEHAVIOR_H
#define LIGHTBEHAVIOR_H

#include <Knob/MotionVector/KnobMotionVector.hpp>
class LightBehavior {
public:
    virtual void applyTo(KnobMotionVector *axis) = 0;
    virtual ~LightBehavior() {}
};

#endif // LIGHTBEHAVIOR_H