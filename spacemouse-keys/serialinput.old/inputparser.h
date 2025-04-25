#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include <Arduino.h>

/**
 * @brief Class to parse input commands from the serial monitor.
 * @details This class handles the input from the serial monitor and parses it into words.
 * @note This class also includes methods to handle different numbers of words in the input.
 * @warning Ensure that the input does not exceed the expected number of words to avoid buffer overflow.
 */
class InputParser {
private:
    int8_t InputParser::_handleOneWord(char *words[]);
    int8_t InputParser::_handleTwoWords(char *words[]);
    int8_t InputParser::_handleThreeWords(char *words[]);

protected:
public:
    InputParser() {};
    ~InputParser() {}

    void Parse();
};

#endif // INPUTPARSER_H