#pragma once

#include "IObserver.hpp"
#include "kinematics/kinematics.h"

class HIDProcessorKey : public IObserver {
public:
    HIDProcessorKey() {};
    ~HIDProcessorKey() {};

    void update(Kinematics *kinematics) override {};
    void update(Hardware *hardware) override {};
    void update(KeyCollection *keyCollection) override {};
};