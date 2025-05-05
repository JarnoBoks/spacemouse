#pragma once
// This Visitor interfacee is used to implement the Visitor pattern for the Sensor and Hardware classes.
// It allows for different types of visitors to be applied to the Sensor and Hardware classes without modifying their code.

// We have to add all the classes that we want to visit here
// - Sensor (for the sensor names)
// - SensorConfig (for the sensor configuration)
// - Hardware (for the hardware type and version)
// - Axis (for the axis type and name)
// - AxisConfig (for the axis configuration)

class Sensor;
class SensorConfig;
class Hardware;
class Axis;
class AxisConfig;
class KinematicsConfig;

class IPrinterVisitor {
public:
    virtual void visit(Sensor &sensor) = 0;
    virtual void visit(SensorConfig &config) = 0;
    virtual void visit(Hardware &hardware) = 0;
    virtual void visit(Axis &axis) = 0;
    virtual void visit(AxisConfig &config) = 0;
    virtual void visit(KinematicsConfig &config) = 0;
    // Add more as needed
};
