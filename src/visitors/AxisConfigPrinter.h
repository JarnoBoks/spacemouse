#pragma once

#include "IPrinterVisitor.h"
#include "axis/axes/Axis.hpp"
#include "axis/config/AxisConfig.hpp"
#include "kinematics/config/kinematicsconfig.hpp"

#define FMT_NOCOMMA false
#define FMT_COMMA true
#define NO_PREFIX ""
#define NO_SIGN " "
#define TWO_DECIMALS 2
#define ONE_DECIMAL 1
#define NO_DECIMALS 0

class AxisConfigPrinter : public IPrinterVisitor {
private:
    void _helper_PrintConfig(const char *name, const float posval, const float negval, const boolean printseparator, const char *typeprefix, const uint8_t precision, const int8_t minwidth);
    size_t _helper_PrintItem(const char *separator, const char *typeprefix, const char *name, const char *sign);

public:
    AxisConfigPrinter() {}

    inline void visit(Axis &axis) override {

        // Print the sensitivity
        _helper_PrintConfig(axis.getName(), axis.getConfig()->posConfig.getSensitivity(), axis.getConfig()->negConfig.getSensitivity(), FMT_NOCOMMA, NO_PREFIX, TWO_DECIMALS, 21);

        // Print the gate
        _helper_PrintConfig(axis.getName(), axis.getConfig()->posConfig.getGate(), axis.getConfig()->negConfig.getGate(), FMT_COMMA, "G", NO_DECIMALS, 21);

        // Print the modfunc
        _helper_PrintConfig(axis.getName(), axis.getConfig()->posConfig.getModFuncType(), axis.getConfig()->negConfig.getModFuncType(), FMT_COMMA, "M", NO_DECIMALS, 19);

        // Print the inversion
        _helper_PrintConfig(axis.getName(), axis.getConfig()->inversion, axis.getConfig()->inversion, FMT_COMMA, "I", NO_DECIMALS, -1);
        Serial.println(); // Print a newline after the last axis status was printed to the serial monitor
    }

    inline void visit(AxisConfig &config) override {}
    inline void visit(Sensor &sensor) override {}
    inline void visit(SensorConfig &config) override {}
    inline void visit(KinematicsConfig &config) override {}
};
