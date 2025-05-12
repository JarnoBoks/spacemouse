#pragma once

/**
 * @brief Interface class for handling key functionality.
 * This class provides a common interface for different types of key functionalities (e.g., command, kill switch).
 * @deprecated
 */
class IKey {
public:
    virtual ~IKey() {}
    virtual void evaluate() = 0; // Pure virtual function to evaluate the key state and call the appropriate functionality
};