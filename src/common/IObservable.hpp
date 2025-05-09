#pragma once

class IObserver; // Forward declaration of IObserver class

class IObservable {
public:
    virtual void attachObserver(IObserver *observer) = 0;
    virtual void detachObserver(IObserver *observer) = 0;
    virtual void clearObservers() = 0;

    virtual void notifyObservers() = 0;
};
