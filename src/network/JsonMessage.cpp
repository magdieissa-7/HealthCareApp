#include "JsonMessage.h"
#include <sstream>
#include <algorithm>



std::string JsonMessage::createLoginMessage(const std::string &username) {
    return "{\"type\":\"login\",\"username\":\"" + username + "\"}";
}

std::string JsonMessage::createEmergencyMessage(const std::string &contact) {
    return "{\"type\":\"emergency\",\"contact\":\"" + contact + "\"}";
}

std::string JsonMessage::createMedicationMessage(const std::string &name,
                                                   const std::string &dosage,
                                                   const std::string &time) {
    return "{\"type\":\"medication\",\"name\":\"" + name +
           "\",\"dosage\":\"" + dosage +
           "\",\"time\":\"" + time + "\"}";
}

std::string JsonMessage::createAckMessage(const std::string &status) {
    return "{\"type\":\"ack\",\"status\":\"" + status + "\"}";
}



std::map<std::string, std::string> JsonMessage::parse(const std::string &json) {
    std::map<std::string, std::string> result;
    std::string s = json;


    auto start = s.find('{');
    auto end   = s.rfind('}');
    if (start == std::string::npos || end == std::string::npos) return result;
    s = s.substr(start + 1, end - start - 1);


    std::istringstream ss(s);
    std::string token;
    while (std::getline(ss, token, ',')) {
        auto colon = token.find(':');
        if (colon == std::string::npos) continue;

        std::string key   = token.substr(0, colon);
        std::string value = token.substr(colon + 1);


        auto stripQuotes = [](std::string &str) {
            str.erase(std::remove(str.begin(), str.end(), '"'), str.end());
            str.erase(str.begin(), std::find_if(str.begin(), str.end(),
                      [](unsigned char c){ return !std::isspace(c); }));
            str.erase(std::find_if(str.rbegin(), str.rend(),
                      [](unsigned char c){ return !std::isspace(c); }).base(), str.end());
        };

        stripQuotes(key);
        stripQuotes(value);

        if (!key.empty()) result[key] = value;
    }
    return result;
}

std::string JsonMessage::getField(const std::string &json, const std::string &field) {
    auto fields = parse(json);
    auto it = fields.find(field);
    return (it != fields.end()) ? it->second : "";
}

bool JsonMessage::isValidMessage(const std::string &json) {
    if (json.empty()) return false;
    auto fields = parse(json);
    return fields.count("type") > 0;
}
