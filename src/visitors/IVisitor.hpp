#pragma once

class Visitable;

class IVisitor {
public:
    virtual void visit(Visitable &visitable) = 0;
};
