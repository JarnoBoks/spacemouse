#pragma once
#include "CalibratorStateBase.hpp"
#include <stdint.h> // For uint16_t type

#ifndef IDLE_ITERATIONS
#define IDLE_ITERATIONS 500 // Default number of iterations for idle calibration during startup
#endif

class CalibratorStateIdle : public CalibratorStateBase {
private:
    const uint16_t m_iterations = IDLE_ITERATIONS; // Number of iterations for idle calibration
    uint16_t m_processedIterations = 0;            // Number of processed iterations for idle calibration

public:
    CalibratorStateIdle() = default;                                             // Default constructor
    CalibratorStateIdle(const uint16_t iterations) : m_iterations(iterations) {} // Parameterized constructor
    virtual ~CalibratorStateIdle() = default;

    void start() override;
    void update() override;
    void finish() override;
};
