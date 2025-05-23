#pragma once

#include <common/IVisitor.hpp>
#include <sensor/sensors/Sensor.hpp>

class SensorNamePrinter : public IVisitor {
public:
    SensorNamePrinter() = default;

    /**
     * @brief Visit the Sensor object and print its name.
     * @param sensor The Sensor object to visit.
     */
    void visit(VisitableBase &visitable) override {
        // Cast the VisitableBase to Sensor
        Sensor &sensor = static_cast<Sensor &>(visitable);

        Serial.print(sensor.getName());
        Serial.print(F(":  "));
    }
};
