#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_AVR)
#if defined(ARDUINO_ARCH_ESP32)

// Include the header files for the command handler that will handle the commands send by the serial interface
#include "commandhandler/EspCommandHandler.hpp"
#include "..\CollectionCarrier\CollectionCarrier.hpp"

// Include the header files for the commands that can be received through the serial interface
#include "commandhandler/debugcommand.h"
#include "commandhandler/idlecommand.h"
#include "commandhandler/minmaxcommand.h"
#include "commandhandler/senscommand.h"
#include "commandhandler/gatecommand.h"
#include "commandhandler/modfunccommand.h"
#include "commandhandler/invertcommand.h"
#include "commandhandler/showcommand.h"
#include "commandhandler/exclusivecommand.h"
#include "commandhandler/switchyzcommand.h"

// Define the command handler type for ESP32
#define CommandHandler ESPCommandHandler

class CommandHandlerFactory {
private:
    CollectionIdentifier *m_Collections; // Pointer to the collection identifier object
public:
    CommandHandlerFactory(CollectionIdentifier *collections) : m_Collections(collections) {
        // Constructor
    }

    ~CommandHandlerFactory() {
        // Destructor
    }

    CommandHandler *createCommandHandler() {
        return new CommandHandler(); // Create a new command handler object
    }

    void setupCommandHandler(CommandHandler *commandHandler) {
        //  Setup the Command Handler and register the commands that can be handled via the serial interface.
        commandHandler->registerCommand(new DebugCommand(m_Collections));
        commandHandler->registerCommand(new IdleCommand(m_Collections));
        commandHandler->registerCommand(new MinMaxCommand(m_Collections));
        commandHandler->registerCommand(new SensCommand());
        commandHandler->registerCommand(new GateCommand());
        commandHandler->registerCommand(new ModFuncCommand());
        commandHandler->registerCommand(new InvertCommand());
        commandHandler->registerCommand(new ShowCommand());
        commandHandler->registerCommand(new ExclusiveCommand());
        commandHandler->registerCommand(new SwitchYZCommand());
    }
};

#endif // ARDUINO_ARCH_ESP32

#if defined(ARDUINO_ARCH_AVR)
#include <commandhandler/AvrCommandHandler/AvrCommandHandler.hpp>

// Define the command handler type for ESP32
#define CommandHandler AVRCommandHandler

class CommandHandlerFactory {
private:
    CollectionCarrier *m_Collections; // Pointer to the collection identifier object
public:
    CommandHandlerFactory(CollectionCarrier *collections) : m_Collections(collections) {
        // Constructor
    }

    ~CommandHandlerFactory() {
        // Destructor
    }

    CommandHandler *createCommandHandler() {
        return new CommandHandler(); // Create a new command handler object
    }

    void setupCommandHandler(CommandHandler *commandHandler) {
        commandHandler->setCollectionIdentifier(m_Collections); // Set the collection identifier for the command handler
    }
};

#endif // ARDUINO_ARCH_AVR

#else
#error "Unsupported architecture"
#endif // ARDUINO_ARCH_ESP32 || ARDUINO_ARCH_AVR