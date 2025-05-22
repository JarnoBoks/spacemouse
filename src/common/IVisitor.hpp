#pragma once

class VisitableBase;

class IVisitor {
public:
    virtual void visit(VisitableBase &visitable) = 0;
};
