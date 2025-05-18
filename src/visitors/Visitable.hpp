#pragma once
#include "IVisitable.hpp" // Include the IVisitable interface header file
#include <stdint.h>

class IVisitor;

class Visitable : public IVisitable {
private:
public:
    // Constructor
    Visitable() = default;

    // Destructor
    virtual ~Visitable() = default;

    // Accept method for the visitor pattern
    void accept(IVisitor &visitor) override;
};