#include "VisitableBase.hpp"
#include <common/IVisitor.hpp>

// Accept method for the visitor pattern
void VisitableBase::accept(IVisitor &visitor) {
    visitor.visit(*this);
}