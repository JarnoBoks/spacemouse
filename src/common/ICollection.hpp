#pragma once

#include "IObservable.hpp" // Include the IObservable interface header file
#include <stdint.h>

class ICollectable;

class ICollection {
public:
    virtual ~ICollection() = default;

    virtual void add(ICollectable *collectable) = 0;
    virtual void remove(ICollectable *collectable) = 0;
    virtual void evaluate() = 0;

    virtual ICollectable *getItem(uint8_t id) const = 0;
    virtual ICollectable *getItem(const char *name) const = 0;
    virtual uint8_t getItemCount() const = 0;
};