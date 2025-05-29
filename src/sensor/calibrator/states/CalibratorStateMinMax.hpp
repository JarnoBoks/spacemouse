#pragma once
#include "CalibratorStateBase.hpp"

class CalibratorStateMinMax : public CalibratorStateBase {
private:
    const bool m_persist = false; // Flag to indicate if the calibration should be persisted in non-volatile storage
public:
    CalibratorStateMinMax() = default;                                // Default constructor
    CalibratorStateMinMax(const bool persist) : m_persist(persist) {} // Parameterized constructor

    virtual ~CalibratorStateMinMax() = default;

    void start() override;
    void update() override;
    void finish() override;
};
