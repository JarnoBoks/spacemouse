#ifndef LIGHTBEHAVIOR_H
#define LIGHTBEHAVIOR_H

#include <knob/axis/KnobAxis.hpp>
class LightBehavior {
public:
    virtual void applyTo(KnobAxis *axis) = 0;
    virtual ~LightBehavior() {}
};

#endif // LIGHTBEHAVIOR_H