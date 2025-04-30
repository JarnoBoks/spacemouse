#pragma once
#ifndef IPRINTERVISITOR_H
#define IPRINTERVISITOR_H
// This Visitor interfacre is used to implement the Visitor pattern for the Sensor and Hardware classes.
// It allows for different types of visitors to be applied to the Sensor and Hardware classes without modifying their code.
#include "sensor/sensor.h"
#include "hardware/hardware.h"

// We have to add all the classes that we want to visit here
// - Sensor (for the sensor names)
// - SensorConfig (for the sensor configuration)
// - Hardware (for the hardware type and version)

class IPrinterVisitor {
public:
    virtual void visit(Sensor &sensor) = 0;
    virtual void visit(SensorConfig &config) = 0;
    virtual void visit(Hardware &hardware) = 0;
    // Add more as needed
};

#endif // IPRINTERVISITOR_H