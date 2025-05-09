#pragma once

// Forward declaration of ICollection class
class ICollection;

class ICollectable {
public:
    virtual ~ICollectable() = default;

    virtual void setContext(ICollection *Collection) {}; // TODO - Make pure virtual
    virtual void evaluate() {};                          // TODO - Make pure virtual
};