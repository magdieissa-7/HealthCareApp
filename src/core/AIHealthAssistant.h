#ifndef AI_HEALTH_ASSISTANT_H
#define AI_HEALTH_ASSISTANT_H

#include <string>
#include <functional>
#include <vector>

// AIHealthAssistant sends patient questions to the Anthropic Claude API
// and returns AI-generated health guidance.
// This satisfies the bonus requirement: "Development of an LLM-powered application"
//
// The interface allows GoogleMock to mock it for unit testing.
class IAIHealthAssistant {
public:
    virtual ~IAIHealthAssistant() = default;
    virtual void ask(const std::string &question,
                     std::function<void(const std::string &answer,
                                        const std::string &error)> callback) = 0;
};

class AIHealthAssistant : public IAIHealthAssistant {
public:
    // apiKey: your Anthropic API key (set via env var ANTHROPIC_API_KEY)
    explicit AIHealthAssistant(const std::string &apiKey);

    // Sends question to Claude API asynchronously.
    // callback(answer, error) — error is empty on success.
    void ask(const std::string &question,
             std::function<void(const std::string &answer,
                                const std::string &error)> callback) override;

private:
    std::string apiKey;

    // Builds the JSON request body for the Anthropic messages API
    std::string buildRequestBody(const std::string &question) const;

    // Parses the response JSON to extract the assistant's text
    std::string parseResponse(const std::string &responseJson) const;
};

#endif // AI_HEALTH_ASSISTANT_H
