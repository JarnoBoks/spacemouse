#pragma once

#include "IObservable.hpp" // Include the IObservable interface header file
#include <stdint.h>

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

    virtual void attachObserver(IObserver *observer) override;
    virtual void detachObserver(IObserver *observer) override;
    virtual void clearObservers() override;

    virtual void notifyObservers() override;
};