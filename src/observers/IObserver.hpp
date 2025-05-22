#pragma once
#include <stdint.h>

class IObservable;

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
};
