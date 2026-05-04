#include "Validator.h"
#include <regex>

bool Validator::isNotEmpty(const std::string &text) {
    return !text.empty();
}

bool Validator::isValidTime(const std::string &time) {
    std::regex pattern("^([01][0-9]|2[0-3]):[0-5][0-9]$");
    return std::regex_match(time, pattern);
}
