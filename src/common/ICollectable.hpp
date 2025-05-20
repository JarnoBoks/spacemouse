#pragma once

class ICollection;

class ICollectable {
public:
    virtual ~ICollectable() = default;

    virtual void setContext(ICollection *Collection) {}; // TODO - Make pure virtual
    virtual void evaluate() = 0;
    virtual const bool hasDescriptor(const char *descriptor) const = 0;
};