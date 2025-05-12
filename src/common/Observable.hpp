#pragma once

#include "IObservable.hpp" // Include the IObservable interface header file
#include <stdint.h>

class IObserver;

class Observable : public IObservable {
    // REFACTOR - Can we use a template function in the base class for all these getItem functions in the collection classes?
    // REFACTOR - Can we add consts to the function parameters?
private:
    IObserver **observers;     // Array of pointers to observers        // FIXME - Create a template for the observers
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

    void attachObserver(IObserver *observer) override;
    void detachObserver(IObserver *observer) override;
    void clearObservers() override;
    virtual void notifyObservers() override;
};