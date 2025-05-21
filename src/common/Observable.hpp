#pragma once

#include "IObservable.hpp" // Include the IObservable interface header file
#include <stdint.h>

class IObserver;

/**
 * @brief Observable class that maintains a list of observers and notifies them of changes.
 */
class Observable : public IObservable {
private:
    IObserver **observers;     // Array of pointers to observers
    uint8_t observerCount = 0; // Number of observers attached
    uint8_t maxObservers;

public:
    Observable() = delete; // Delete the default constructor to prevent instantiation without parameters
    Observable(uint8_t maxObservers) : maxObservers(maxObservers) {
        observers = new IObserver *[maxObservers]; // Allocate memory for the observers array

        for (uint8_t i = 0; i < maxObservers; i++) {
            observers[i] = nullptr; // Initialize the observers array to nullptr
        }
    }

    virtual ~Observable() {
        clearObservers(); // Clear the observers when the observable is destroyed
        delete[] observers;
    }

    void attachObserver(IObserver *observer);
    void detachObserver(IObserver *observer);
    void clearObservers() override;
    virtual void notifyObservers() override; // REVIEW - Should this be public or protected? The class knows if there has to be notification. // REVIEW - Why are all these functions virtual? Necessary to override by Derived classes?
};