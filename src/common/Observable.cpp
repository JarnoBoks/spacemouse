#include "Observable.hpp"
#include "observers/IObserver.hpp"
#include <stdint.h>

/**
 * @brief Attach an observer to the hardware class.
 * @param observer Pointer to the observer to be attached.
 * @details This function adds the observer to the observers array and increases the observer count.
 *          If the array is full, it does not add the new observer and can be modified to handle this case.
 */
// REFACTOR - Can we add consts to the function parameters?
void Observable::attachObserver(IObserver *observer) {
    if (observerCount < maxObservers && observer != nullptr) {
        // insert the observer into the array, at position observerCount and increase the count after inserting.
        observers[observerCount++] = observer;
    } else {
        // TODO - Handle the case when the observer array is full. Maybe remove the oldest observer or ignore the new one?
    }
};

// REFACTOR - Can we add consts to the function parameters?
void Observable::detachObserver(IObserver *observer) {
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
void Observable::notifyObservers() {
    for (uint8_t i = 0; i < observerCount; i++) {
        observers[i]->update(this); // Notify each observer, with the observable instance as parameter
    }
}

void Observable::clearObservers() {
    for (uint8_t i = 0; i < observerCount; i++) {
        observers[i] = nullptr;
    }
    observerCount = 0;
}