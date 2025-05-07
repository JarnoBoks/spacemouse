class IHIDCommand {
public:
    virtual void execute() = 0;
    virtual ~IHIDCommand() {}
};