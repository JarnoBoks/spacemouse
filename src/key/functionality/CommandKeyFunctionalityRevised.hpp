#pragma once
#include "IKeyFunctionality.h"
#include "hidhandler/commands/ICommand.hpp" // For Command interface

class CommandKeyFunctionalityRevised : public IKeyFunctionality {
private:
    ICommand *m_Command = nullptr; // Pointer to the command object associated with this key functionality

public:
    CommandKeyFunctionalityRevised() = delete; // Default constructor not allowed, keycontext needs to be initialized
    CommandKeyFunctionalityRevised(ICommand *cmd) : m_Command(cmd) {}

    inline void onPress() override {
        m_Command->execute();
    };

    inline void onRelease() override {
        m_Command->execute();
    };
};
