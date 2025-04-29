class SerialPrintVisitor : public IPrintableVisitor {
public:
    void visit(Sensor &sensor) override {
        Serial.print(sensor.getName());
        // ... print more info
    }
    void visit(SensorConfig &config) override {
        // ... print config info
    }
    void visit(Hardware &hardware) override {
        // ... print hardware info
    }
};