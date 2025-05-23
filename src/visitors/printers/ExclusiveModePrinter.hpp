#pragma once

#include <common/IVisitor.hpp>
#include "sensor/sensors/Sensor.hpp"
#include "sensor/config/SensorConfig.hpp"
#include "kinematics/config/kinematicsconfig.hpp"
#include "common/TextHelper.h"

class ExclusiveModePrinter : public IVisitor {
public:
    ExclusiveModePrinter() {
        // Constructor to initialize the ExclusiveModePrinter object
        Serial.println(F("Exclusive Mode: "));
    }

    void visit(VisitableBase &visitable) override {
        // Cast the VisitableBase to KinematicsConfig
        KinematicsConfig &config = static_cast<KinematicsConfig &>(visitable);

        bool exclusiveMode = config.getExclusiveMode(); // Get the minimum value from the sensor configuration
        Serial.print(exclusiveMode);
        TextHelper::printBooleanDescription(exclusiveMode);
        Serial.println();
    }
};
