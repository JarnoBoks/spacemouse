#pragma once
#include <stdint.h>

// Define maximum observers for Kinematics class
#define MAX_KINEMATICS_OBSERVERS 4

// Define maximum observers for Hardware class
#define MAX_HARDWARE_OBSERVERS 4

// Define maximum observers for KeyCollection class
#define MAX_KEYCOLLECTION_OBSERVERS CFG_NUMBER_OF_KEYS // TODO - This should be the number of command keys, not the number of keys!

// Define maximum observers for AxisCollection class
// FIXME - Replaces the MAX_KINEMATICS_OBSERVERS definition
constexpr uint8_t cOBSV_MAX_AXISCOLLECTION_OBSERVERS = 4;

// Simple checks for the configuration above.
#if MAX_KEYCOLLECTION_OBSERVERS < CFG_NUMBER_OF_KEYS
#error "The number of observers for KeyCollection is less than the number of keys!"
#endif
#if MAX_KINEMATICS_OBSERVERS > 255 || MAX_HARDWARE_OBSERVERS > 255 || MAX_KEYCOLLECTION_OBSERVERS > 255
#error "The number of observers is too high!"
#endif

// Forward declaration of classes that are observed (to avoid circular dependencies)
class Kinematics;
class Hardware;
class KeyCollection;
class IObservable; // REVIEW - Can probably be removed, not using the Observable interface in the observer class

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
    virtual void update(IObservable *observable) {}; // Default implementation for IObservable, can be overridden by derived classes
    // TODO virtual void update(IObservable *observable) {}; // Default implementation for IObservable, can be overridden by derived classes
};
