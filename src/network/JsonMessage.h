#ifndef JSON_MESSAGE_H
#define JSON_MESSAGE_H
#include <string>
#include <map>

// JsonMessage handles both SERIALIZATION (building JSON strings)
// and DESERIALIZATION (parsing JSON strings back into data).
class JsonMessage {
public:
    // ── Serialization ────────────────────────────────────────────────────────
    static std::string createLoginMessage(const std::string &username);
    static std::string createEmergencyMessage(const std::string &contact);
    static std::string createMedicationMessage(const std::string &name,
                                                const std::string &dosage,
                                                const std::string &time);
    static std::string createAckMessage(const std::string &status);

    // ── Deserialization ──────────────────────────────────────────────────────
    // Parses a JSON string and returns a key->value map.
    // e.g. {"type":"login","username":"alice"} -> {{"type","login"},{"username","alice"}}
    static std::map<std::string, std::string> parse(const std::string &json);

    // Extracts a single field value from a JSON string.
    // Returns empty string if the field is not found.
    static std::string getField(const std::string &json, const std::string &field);

    // Validates that a JSON string contains a required field with a given value.
    static bool isValidMessage(const std::string &json);
};

#endif // JSON_MESSAGE_H
