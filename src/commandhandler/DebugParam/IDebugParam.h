#ifndef DP_IDEBUGPARAM_H
#define DP_IDEBUGPARAM_H

/**
 * @brief Class IDebugState
 * @details This is an abstract base class that defines the interface for different debug states. The derived classed handle the DEBUG command parameter.
 * @note Implementing classes must provide definitions for the apply and report methods.
 */
class IDebugParam {
public:
    virtual void apply() = 0;
    virtual void report() = 0;
    virtual ~IDebugParam() {}
};

#endif // DP_IDEBUGPARAM_H