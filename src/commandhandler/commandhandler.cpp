#include "CommandHandler.h"

#define MAX_INPUT_SIZE 64 // Maximum size of the input buffer

/**
 * @brief CommandHandler constructor.
 * @details Initializes the command handler with a null pointer for the commands array.
 */
CommandHandler::CommandHandler() : commands{nullptr} {}

/**
 * @brief Registers a command in the command handler.
 * @details Stores the command pointer in the commands array at the specified index.
 * @param index The index to store the command in the commands array.
 * @param cmd The command pointer to be stored.
 */
void CommandHandler::registerCommand(int index, ICommand *cmd) {
    if (index >= 0 && index < MAX_COMMANDS)
        commands[index] = cmd;
}

/**
 * @brief Handles the input received from the serial monitor.
 * @details Parses the input string and executes the corresponding command.
 * @param input The input string received from the serial monitor.
 * @param size The size of the input string.
 * @param bytesRead The number of bytes read from the input.
 */
void CommandHandler::handleInput(char input[], const uint8_t inputsize, const int8_t bytesRead) {
    if (bytesRead == 0) {
        return; // No input received, exit the function
    }

    // Retrieve the command name and parameters from the input
    // Assuming input is a text like "command param1 param2"

    char *wordPtr = NULL;   // Pointer to the current word buffer
    char *words[3] = {};    // Array of word pointers
    uint8_t paramCount = 0; // Initialize paramCount

    wordPtr = strtok(input, " ");                                // Tokenize the input string by spaces
    while (wordPtr != NULL && wordPtr - input < inputsize - 1) { // cast to char to prevent signed/unsigned comparison. Will fit easily due to the buffer size of 64.
        words[paramCount++] = wordPtr;                           // Store the token in the words array and increment paramCount
        if (paramCount >= 3) {
            break; // Stop if we have processed three words
        }
        wordPtr = strtok(NULL, " "); // Get the next token
    }

    // TODO - Necessary to check for empty command?

    // Loop through the commands array to find the command we want to process.
    // FIXME - MAX_COMMANDS is not the correct size. We need to check for the command count or nullpointer.
    for (int i = 0; i < MAX_COMMANDS; ++i) {

        if (commands[i] && commands[i]->isCommand(words[0])) {      // Check if the command name matches) {
            commands[i]->execute(words[1], words[2], --paramCount); // Execute the command with the parameters
            return;
        }
    }
}

/**
 * @brief Parses the input received from the serial monitor.
 * @details Reads the input from the serial monitor and stores it in a buffer.
 *          The input is terminated by a newline character or when the buffer is full.
 */
void CommandHandler::parseSerialMonitorInput() {
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