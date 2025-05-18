#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_AVR) // Check is necessary to generate a compile error if the code is not compiled for ESP32 or AVR
#if defined(ARDUINO_ARCH_ESP32)

// Include the header files for the command handler that will handle the commands send by the serial interface
#include "commandhandler/EspCommandHandler/EspCommandHandler.hpp"
#include "commandhandler/CollectionCarrier/CollectionCarrier.hpp"

// Include the header files for the commands that can be received through the serial interface
#include "commandhandler/EspCommandHandler/commands/debugcommand.h"
#include "commandhandler/EspCommandHandler/commands/idlecommand.h"
#include "commandhandler/EspCommandHandler/commands/minmaxcommand.h"
#include "commandhandler/EspCommandHandler/commands/senscommand.h"
#include "commandhandler/EspCommandHandler/commands/gatecommand.h"
#include "commandhandler/EspCommandHandler/commands/modfunccommand.h"
#include "commandhandler/EspCommandHandler/commands/invertcommand.h"
#include "commandhandler/EspCommandHandler/commands/showcommand.h"
#include "commandhandler/EspCommandHandler/commands/exclusivecommand.h"
#include "commandhandler/EspCommandHandler/commands/switchyzcommand.h"

// Define the command handler type for ESP32
#define CommandHandler ESPCommandHandler

class CommandHandlerFactory {
private:
    CollectionCarrier *m_Collections; // Pointer to the collection carrier object
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
        //  Setup the Command Handler and register the commands that can be handled via the serial interface.
        commandHandler->registerCommand(new DebugCommand(m_Collections));
        commandHandler->registerCommand(new IdleCommand(m_Collections));
        commandHandler->registerCommand(new MinMaxCommand(m_Collections));
        commandHandler->registerCommand(new SensCommand(m_Collections));
        commandHandler->registerCommand(new GateCommand(m_Collections));
        commandHandler->registerCommand(new ModFuncCommand(m_Collections));
        commandHandler->registerCommand(new InvertCommand(m_Collections));
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