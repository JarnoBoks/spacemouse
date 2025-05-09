#ifndef LIGHTBEHAVIOR_H
#define LIGHTBEHAVIOR_H

#include "axis/Axis.hpp"
class LightBehavior {
public:
    virtual void applyTo(Axis *axis) = 0;
    virtual ~LightBehavior() {}
};

#endif // LIGHTBEHAVIOR_H