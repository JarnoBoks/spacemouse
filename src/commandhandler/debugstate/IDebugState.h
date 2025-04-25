#ifndef IDEBUGSTATE_H
#define IDEBUGSTATE_H

class IDebugState {
public:
    virtual void apply() = 0;
    virtual void report() = 0;
    virtual ~IDebugState() {}
};

#endif // IDEBUGSTATE_H