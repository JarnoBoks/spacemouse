#pragma once

// Define maximum observers for Kinematics class
#define MAX_KINEMATICS_OBSERVERS 4

// Define maximum observers for Hardware class
#define MAX_HARDWARE_OBSERVERS 4

// Forward declaration of classes that are observed (to avoid circular dependencies)
class Kinematics;
class Hardware;
class KeyCollection;
class IObservable;

/**
 * @brief Interface for observer classes that respond to updates from observed objects.
 * @details This interface defines the methods that must be implemented by any observer class.
 *          Observers will receive updates from the observed objects they are registered with.
 */
class IObserver {

public:
    IObserver() = default; // Default constructor
    // NOTE IObserver(const IObserver &) = delete; // Delete copy constructor to prevent copying
    virtual ~IObserver() {}

    virtual void update(Kinematics *kinematics);
    virtual void update(Hardware *hardware);
    virtual void update(KeyCollection *keyCollection);
};
