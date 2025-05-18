#pragma once

#include "IDebugParam.hpp"
#include <commandhandler/EspCommandHandler/commands/DebugCommand.h> // For DebugCommand

/**
 * @brief Base class for debug parameters.
 */
class DebugParam : public IDebugParam {
protected:
    DebugCommand *m_Context = nullptr; // Pointer to the command context
public:
    DebugParam() = delete;                                    // Constructor
    DebugParam(DebugCommand *context) : m_Context(context) {} // Constructor to initialize the command context
    virtual ~DebugParam() {};                                 // Destructor to clean up the observer instance

    void set_context(DebugCommand *context) { m_Context = context; } // Set the command context

    void apply() override;
    void report() override;
};