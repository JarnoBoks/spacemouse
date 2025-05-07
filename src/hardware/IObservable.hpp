#pragma once
#include <stdint.h> // Include for uint8_t type
class IObserver;    // Forward declaration of IObserver class

class IObservable {
public:
    virtual void attachObserver(IObserver *observer) = 0;
    virtual void detachObserver(IObserver *observer) = 0;
    virtual void notifyObservers() = 0;
};
