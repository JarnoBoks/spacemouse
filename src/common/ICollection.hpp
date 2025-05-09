#pragma once

#include "IObservable.hpp" // Include the IObservable interface header file

// Forward declaration of ICollectable class
class ICollectable;

class ICollection : public IObservable {
public:
    virtual ~ICollection() = default;

    virtual void add(ICollectable *collectable) = 0;
    virtual void remove(ICollectable *collectable) = 0;
};