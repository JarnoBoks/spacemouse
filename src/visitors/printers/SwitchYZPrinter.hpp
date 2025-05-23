#pragma once

#include <common/IVisitor.hpp>
#include "kinematics/config/kinematicsconfig.hpp"
#include "common/TextHelper.h"

class SwitchYZPrinter : public IVisitor {
public:
    SwitchYZPrinter() {
        // Constructor to initialize the SwitchYZPrinter object
        Serial.println(F("SwitchYZ: "));
    }

    void visit(VisitableBase &visitable) override {
        // Cast the VisitableBase to KinematicsConfig
        KinematicsConfig &config = static_cast<KinematicsConfig &>(visitable);

        bool switchYZ = config.getSwitchYZ(); // Get the YZ switching state from the kinematics configuration
        Serial.print(switchYZ);
        TextHelper::printBooleanDescription(switchYZ);
        Serial.println();
    }
};
