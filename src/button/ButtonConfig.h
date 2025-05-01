// ButtonConfig.h
#pragma once
// REMOVE #include <string>

class ButtonConfig {
protected:
    int id;
    const char *label;
    bool enabled;

public:
    ButtonConfig() : id(0), label(""), enabled(true) {}
    virtual ~ButtonConfig() = default;

    void setId(int id_) { id = id_; }
    int getId() const { return id; }

    void setLabel(const char *&l) { label = l; }
    const char *getLabel() const { return label; }

    void setEnabled(bool e) { enabled = e; }
    bool isEnabled() const { return enabled; }
};

class PhysicalButtonConfig : public ButtonConfig {
    int pinNumber;
    int debounceTime;

public:
    PhysicalButtonConfig() : pinNumber(0), debounceTime(50) {}
    void setPinNumber(int pin) { pinNumber = pin; }
    int getPinNumber() const { return pinNumber; }

    void setDebounceTime(int ms) { debounceTime = ms; }
    int getDebounceTime() const { return debounceTime; }
};

class SimulatedButtonConfig : public ButtonConfig {
    const char *simulationKey;

public:
    void setSimulationKey(const char *key) { simulationKey = key; }
    const char *getSimulationKey() const { return simulationKey; }
};