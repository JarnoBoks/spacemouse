#pragma once
class IVisitor; // Forward declaration of IVisitor class

/**
 * @brief Interface for visitable objects that can be visited by a visitor.
 */
class IVisitable {
public:
    virtual void accept(IVisitor &visitor) = 0;
};
