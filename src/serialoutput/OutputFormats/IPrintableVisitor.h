class IPrintableVisitor {
public:
    virtual void visit(Sensor &sensor) = 0;
    virtual void visit(SensorConfig &config) = 0;
    virtual void visit(Hardware &hardware) = 0;
    // Add more as needed
};