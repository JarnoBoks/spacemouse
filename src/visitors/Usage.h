SerialPrintVisitor printer;
sensor.accept(printer);
config.accept(printer);
hardware.accept(printer);

Add an accept method to each source

    class Sensor {
public:
    void accept(IPrintableVisitor &visitor) { visitor.visit(*this); }
    // ...
};