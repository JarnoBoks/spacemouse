#include "ESPCommandHandler.hpp"
#include "commandhandler/CommandBase.h" // For CommandBase class

#define MAX_INPUT_SIZE 48 // Maximum size of the input buffer, ie. maximum number of characters in the user entered command string

/**
 * @brief Constructor for the ESPCommandHandler class.
 */
ESPCommandHandler::ESPCommandHandler() {
    for (uint8_t i = 0; i < MAX_COMMANDS; ++i) {
        commands[i] = nullptr; // Initialize the command pointers to nullptr
    }
}

/**
 * @brief Destructor for the ESPCommandHandler class.
 */
ESPCommandHandler::~ESPCommandHandler() {
    for (int i = 0; i < commandCount; ++i) {
        delete commands[i]; // Clean up the command pointers
    }
}

/**
 * @brief Registers a command in the command handler.
 * @details Stores the command pointer in the commands array at the specified index.
 * @param index The index to store the command in the commands array.
 * @param cmd The command pointer to be stored.
 * @return True if the command was registered successfully, false otherwise.
 */
bool ESPCommandHandler::registerCommand(CommandBase *cmd) {
    if (commandCount >= MAX_COMMANDS) {
        // TODO ESPPRINT- Serial.println(F("CommandHandler: Command array is full!"));  // Print error message if the array is full
        return false; // Exit the function if the array is full
    }
    commands[commandCount++] = cmd; // Store the command pointer in the commands array and increment the command count
    return true;
}

/**
 * @brief Handles the input received from the serial monitor.
 * @details Parses the input string and executes the corresponding command.
 * @param input The input string received from the serial monitor.
 * @param inputsize The size of the input string.
 * @param bytesRead The number of bytes read from the input.
 */
void ESPCommandHandler::handleInput(char input[], const uint8_t inputsize, const int8_t bytesRead) {
    if (bytesRead == 0) {
        return; // No input received, exit the function
    }

    // Retrieve the command name and parameters from the input
    // Assuming input is a text like "command param1 param2"

    char *wordPtr = NULL;  // Pointer to the current word buffer
    char *words[3] = {};   // Array of word pointers
    uint8_t wordCount = 0; // The number of words/items that are in the input string.

    wordPtr = strtok(input, " ");                                // Tokenize the input string by using spaces as the delimiter.
    while (wordPtr != NULL && wordPtr - input < inputsize - 1) { // cast to char to prevent signed/unsigned comparison. Will fit easily due to the buffer size of 64.
        words[wordCount++] = wordPtr;                            // Store the token in the words array and increment wordCount
        if (wordCount >= 3) {
            break; // Stop if we have processed three words
        }
        wordPtr = strtok(NULL, " "); // Get the next token
    }

    // REVIEW - Necessary to check for empty command?

    // Loop through the commands array to find the command to execute.
    for (uint8_t i = 0; i < commandCount; ++i) {

        if (commands[i] && commands[i]->isCommand(words[0])) {

            // There can be a running command. If the found command is different from the last one executed,
            // stop the execution of the last executed command
            if (i != lastCommandIndex && lastCommandIndex >= 0) {
                commands[lastCommandIndex]->stop();
            }
            lastCommandIndex = i;                                  // Update the last command index
            commands[i]->execute(words[1], words[2], --wordCount); // Execute the command with the retrieved parameters (wordCount decremented, while the first word is the command name)
            return;
        }
    }
}

/**
 * @brief   Parses the input received from the serial monitor into a buffer and calls the handleInput function to process the input.
 * @details Reads the input from the serial monitor and stores it in a buffer.
 *          The input is terminated by a newline character or when the buffer is full.
 */
void ESPCommandHandler::parseSerialMonitorInput() {
    char inputBuffer[MAX_INPUT_SIZE] = ""; // Buffer to store the input command
    uint8_t bytesRead = 0;
    // Read the input into the buffer until a newline character or buffer limit
    while (Serial.available() > 0 && bytesRead < sizeof(inputBuffer) - 1) {
        //  Read a character from the serial input
        char c = Serial.read();
        if (c == '\n') {
            break; // Stop reading at newline
        }
        if (c == '\r') {
            continue; // Ignore carriage return
        }
        inputBuffer[bytesRead++] = toupper(c);
    }
#if ARDUINO_ARCH_ESP32
    if (bytesRead == sizeof(inputBuffer) - 1) {
        Serial.println(F("Reduce command!"));
        return; // Buffer is full, exit the function
    }
#endif

    inputBuffer[bytesRead] = '\0'; // Null-terminate the string

#ifndef ARDUINO_ARCH_AVR
    Serial.print(F("---- Received: ")); // Print the input string
    Serial.println(inputBuffer);        // Print the input string
#endif

    handleInput(inputBuffer, sizeof(inputBuffer), bytesRead); // Call the handleInput function to process the input
}