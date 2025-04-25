#ifndef DEBUGOFF_H
#define DEBUGOFF_H
#include "IDebugState.h"

class DebugOff : public IDebugState {
public:
    void apply() override;
    void report() override;
};
#endif // DEBUGOFF_H