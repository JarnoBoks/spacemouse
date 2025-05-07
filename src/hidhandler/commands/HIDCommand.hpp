#pragma once
#include "ICommand.hpp"

class HIDCommand : public ICommand {
public:
    virtual void execute() = 0;
    virtual ~HIDCommand() {}
};