#pragma once

/**
 * @brief Interface for the Printer Visitor pattern.
 * @details This interface defines the methods that must be implemented by any class that wants to visit the Sensor, SensorConfig, Hardware, Axis, AxisConfig, and KinematicsConfig classes.
 *          This visitor is used to implement the (singular) Serial monitor output for various classes. Concrete Printers classes will output settings or values of the classes they visit
 *          to the Serial monitor.
 * @note    Additional visitor methods (ie. additional classes to visit) can be added as needed for future extensions.
 * @note    This pattern is used for singular output, as opposed to the Observer pattern, which is used for multiple outputs (each loop).
 */
class Sensor;
class SensorConfig;
class Hardware;
class KnobMotionVector;
class AxisConfig;
class KinematicsConfig;

// REFACTOR - We should not need to include all these classes here. We should be able to make one interface for all the classes that need to be visited.
class IPrinterVisitor {
public:
    virtual void visit(Sensor &sensor) = 0;
    virtual void visit(SensorConfig &config) = 0;
    virtual void visit(KnobMotionVector &axis) = 0;
    virtual void visit(AxisConfig &config) = 0;
    virtual void visit(KinematicsConfig &config) = 0;
    // Add more as needed
};
