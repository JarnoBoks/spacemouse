#pragma once
#include <stdint.h>

// Forward declaration of classes that are observed (to avoid circular dependencies)
class Kinematics;
class KeyCollection;
class AxisCollection;
class SensorCollection;
class AxisRotation;
class AxisTranslation;

class IObservable; // REVIEW - Can probably be removed, not using the Observable interface in the observer class

/**
 * @brief Interface for observer classes that respond to updates from observed objects.
 * @details This interface defines the methods that must be implemented by any observer class.
 *          Observers will receive updates from the observed objects they are registered with.
 */
class IObserver {

public:
    IObserver() = default; // Default constructor
    virtual ~IObserver() {}

    inline virtual void update(IObservable *observable) {}; // Default implementation for IObservable, can be overridden by derived classes

    // REFACTOR - Remove the following methods, they should be replaced by the IObservable interface and can be implemented in the derived classes
    inline virtual void update(Kinematics *kinematics) {};
    inline virtual void update(KeyCollection *keyCollection) {};
    inline virtual void update(AxisCollection *axisCollection) {};
    inline virtual void update(SensorCollection *sensorCollection) {};
    inline virtual void update(AxisRotation *axisRotation) {};
    inline virtual void update(AxisTranslation *axisTranslation) {};
};
