#include "Visitable.hpp"
#include "IVisitor.hpp" // Include the IVisitor interface header file

// Accept method for the visitor pattern
void Visitable::accept(IVisitor &visitor) {
    visitor.visit(*this);
}