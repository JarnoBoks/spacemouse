#pragma once
#include <common\IVisitable.hpp> // Include the IVisitable interface header file
#include <stdint.h>

class IVisitor;

class VisitableBase : public IVisitable {
private:
public:
    // Constructor
    VisitableBase() = default;

    // Destructor
    virtual ~VisitableBase() = default;

    // Accept method for the visitor pattern
    void accept(IVisitor &visitor) override;
};