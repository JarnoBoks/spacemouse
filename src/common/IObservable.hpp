#pragma once
class IObserver; // Forward declaration of IObserver class

// REFACTOR - Remove this class
/**
 * @brief Interface for observable objects that can have observers attached.
 * @deprecated
 */
class IObservable {
public:
    virtual void attachObserver(IObserver *observer) = 0;
    virtual void detachObserver(IObserver *observer) = 0;
    virtual void clearObservers() = 0;

    virtual void notifyObservers() = 0;
};