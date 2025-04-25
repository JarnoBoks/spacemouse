#ifndef LIGHTBEHAVIOR_H
#define LIGHTBEHAVIOR_H

#include "kinematics/axis.h"
class LightBehavior {
public:
    virtual void applyTo(Axis *axis) = 0;
    virtual ~LightBehavior() {}
};

#endif // LIGHTBEHAVIOR_H