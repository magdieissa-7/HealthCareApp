#ifndef VALIDATOR_H
#define VALIDATOR_H
#include <string>

class Validator {
public:
    static bool isNotEmpty(const std::string &text);
    static bool isValidTime(const std::string &time);
};

#endif // VALIDATOR_H
