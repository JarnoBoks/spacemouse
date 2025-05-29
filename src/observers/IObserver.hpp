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

    virtual void update(IObservable *observable) = 0; // Pure virtual function to be implemented by derived classes
};
