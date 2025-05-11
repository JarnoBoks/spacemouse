#pragma once

#include "IObservable.hpp" // Include the IObservable interface header file
#include <stdint.h>

// class Observable : public IObservable {
template <class T>
class Observable2 : public IObservable {
    // TODO - Add a template parameter for the observer type
    // TODO - Add a template parameter for the observable type
    // TODO - Add a template parameter for the observer count
    // TODO - Add a template parameter for the max observers
private:
    T **observers;             // Array of pointers to observers
    uint8_t observerCount = 0; // Number of observers attached
    uint8_t maxObservers;

public:
    Observable2() = delete; // Delete the default constructor to prevent instantiation without parameters
    Observable2(uint8_t maxObservers) : maxObservers(maxObservers) {
        observers = new T *[maxObservers]; // Allocate memory for the observers array

        for (uint8_t i = 0; i < maxObservers; i++) {
            observers[i] = nullptr; // Initialize the observers array to nullptr
        }
    }

    virtual ~Observable2() {
        clearObservers(); // Clear the observers when the observable is destroyed
        delete[] observers;
    }

    /**
     * @brief Attach an observer to the hardware class.
     * @param observer Pointer to the observer to be attached.
     * @details This function adds the observer to the observers array and increases the observer count.
     *          If the array is full, it does not add the new observer and can be modified to handle this case.
     */
    // REFACTOR - Can we add consts to the function parameters?
    void attachObserver(T *observer) {
        if (observerCount < maxObservers && observer != nullptr) {
            // insert the observer into the array, at position observerCount and increase the count after inserting.
            observers[observerCount++] = observer;
        } else {
            // TODO - Handle the case when the observer array is full. Maybe remove the oldest observer or ignore the new one?
        }
    };

    // REFACTOR - Can we add consts to the function parameters?
    void detachObserver(T *observer) {
        // remove the observer from the array by replacing it with the last observer in the array and decrease the count.
        for (uint8_t i = 0; i < observerCount; i++) {
            if (observers[i] == observer) {
                observerCount--; // Decrease the observer count
                if (observerCount > 0) {
                    // Move the last observer to the current position
                    observers[i] = observers[observerCount];
                }
                observers[observerCount] = nullptr;
                break;
            }
        }
    };

    /**
     * @brief   Notify all attached observers of a state change.
     * @details This function iterates through the observers array and calls the update method of each observer.
     *          This allows all observers to be informed of changes in the state of the Observable.
     * @note    This function should be called whenever there is a (significant) change in the state of the Observable that observers need to be aware of.
     * @warning Ensure that observers are properly attached before calling this function to avoid null pointer dereferences.
     */
    void notifyObservers() {
        for (uint8_t i = 0; i < observerCount; i++) {
            observers[i]->update(this); // Notify each observer, with the observable instance as parameter
        }
    }

    void clearObservers() {
        for (uint8_t i = 0; i < observerCount; i++) {
            observers[i] = nullptr;
        }
        observerCount = 0;
    }
};