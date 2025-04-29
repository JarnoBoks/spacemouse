#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "axis.h"

// --- Kinematics Singleton ---
class Kinematics {
private:
    static Kinematics *instance;
    Axis axes[6];
    Kinematics();

public:
    static Kinematics *getInstance();

    Axis *getAxis(const AxisType_t type);
    Axis *getAxis(const char *name);

    void processKinematics();
};

#endif // KINEMATICS_H