#pragma once

class Key;

/**
 * @brief Interface for key functionality management.
 */
class StrategyBase {
private:
    Key *m_context = nullptr; // Pointer to the key associated with this functionality

protected:
    Key *getContext() const { return m_context; }
    void setContext(Key *context) { m_context = context; }

public:
    StrategyBase() = default;                          // Default constructor
    StrategyBase(Key *context) : m_context(context) {} // Constructor with context
    virtual ~StrategyBase() = default;

    virtual void onPress() = 0;   // Pure virtual function to handle key press events
    virtual void onRelease() = 0; // Pure virtual function to handle key release events
};