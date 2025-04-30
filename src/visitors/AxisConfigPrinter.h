#pragma once

#include "IPrinterVisitor.h"
#include "axis/axis.h"
#include "axis/axisconfig.h"
#include "hardware/hardware.h"
#include "kinematics/kinematicsconfig.h"
#include "TextHelper.h"

#define FMT_NOCOMMA false
#define FMT_COMMA true
#define NO_PREFIX ""
#define NO_SIGN " "
#define TWO_DECIMALS 2
#define ONE_DECIMAL 1
#define NO_DECIMALS 0

class AxisConfigPrinter : public IPrinterVisitor {
private:
    void PrintConfig();
    void _helper_PrintConfig(const char *name, const float posval, const float negval, const boolean printseparator, const char *typeprefix, const uint8_t precision, const int8_t minwidth);
    size_t _helper_PrintItem(const char *separator, const char *typeprefix, const char *name, const char *sign);

public:
    AxisConfigPrinter() {
        // Constructor to initialize the AxisConfigPrinter object
        // Print the header for the min/max output
        // Serial.println(F("        Min |  Max | Rnge | Warning"));
    }

    inline void visit(Axis &axis) override {

        // Print the sensitivity
        _helper_PrintConfig(axis.getName(), axis.getConfig()->posConfig.sensitivity, axis.getConfig()->negConfig.sensitivity, FMT_NOCOMMA, NO_PREFIX, TWO_DECIMALS, 21);

        // Print the gate
        _helper_PrintConfig(axis.getName(), axis.getConfig()->posConfig.gate, axis.getConfig()->negConfig.gate, FMT_COMMA, "G", NO_DECIMALS, 21);

        // Print the modfunc
        _helper_PrintConfig(axis.getName(), axis.getConfig()->posConfig.modFuncType, axis.getConfig()->negConfig.modFuncType, FMT_COMMA, "M", NO_DECIMALS, 19);

        // Print the inversion
        _helper_PrintConfig(axis.getName(), axis.getConfig()->inversion, axis.getConfig()->inversion, FMT_COMMA, "I", NO_DECIMALS, -1);
        Serial.println(); // Print a newline after the last axis status was printed to the serial monitor
    }

    inline void visit(AxisConfig &config) override {}
    inline void visit(Hardware &hardware) override {}
    inline void visit(Sensor &sensor) override {}
    inline void visit(SensorConfig &config) override {}
    inline void visit(KinematicsConfig &config) override {}
};
