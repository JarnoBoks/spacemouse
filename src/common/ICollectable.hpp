#pragma once

class ICollection;

class ICollectable {
public:
    virtual ~ICollectable() = default;

    virtual void setContext(ICollection *Collection) {}; // TODO - Make pure virtual
    virtual void evaluate() = 0;
    virtual const bool isCurrent(const char *name) const = 0;
};