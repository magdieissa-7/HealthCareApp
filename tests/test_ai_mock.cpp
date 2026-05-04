#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/core/AIHealthAssistant.h"

using ::testing::_;
using ::testing::Invoke;


class MockAIHealthAssistant : public IAIHealthAssistant {
public:
    MOCK_METHOD(void, ask,
        (const std::string &question,
         std::function<void(const std::string &, const std::string &)> callback),
        (override));
};



TEST(AIAssistantMockTest, AskIsCalledWithQuestion) {
    MockAIHealthAssistant mock;
    std::string captured;

    EXPECT_CALL(mock, ask(_, _))
        .WillOnce(Invoke([&captured](
            const std::string &q,
            std::function<void(const std::string&, const std::string&)> cb)
        {
            captured = q;
            cb("Here is health info about your question.", "");
        }));

    mock.ask("What are the side effects of Aspirin?",
             [](const std::string &, const std::string &) {});

    EXPECT_EQ(captured, "What are the side effects of Aspirin?");
}

TEST(AIAssistantMockTest, SuccessCallbackReceivesAnswer) {
    MockAIHealthAssistant mock;
    std::string receivedAnswer;

    EXPECT_CALL(mock, ask(_, _))
        .WillOnce(Invoke([](
            const std::string &,
            std::function<void(const std::string&, const std::string&)> cb)
        {
            cb("Aspirin can cause stomach irritation.", "");
        }));

    mock.ask("Tell me about Aspirin",
             [&receivedAnswer](const std::string &answer, const std::string &) {
                 receivedAnswer = answer;
             });

    EXPECT_EQ(receivedAnswer, "Aspirin can cause stomach irritation.");
}

TEST(AIAssistantMockTest, ErrorCallbackReceivesError) {
    MockAIHealthAssistant mock;
    std::string receivedError;

    EXPECT_CALL(mock, ask(_, _))
        .WillOnce(Invoke([](
            const std::string &,
            std::function<void(const std::string&, const std::string&)> cb)
        {
            cb("", "Network error: Connection refused");
        }));

    mock.ask("Any question",
             [&receivedError](const std::string &, const std::string &error) {
                 receivedError = error;
             });

    EXPECT_FALSE(receivedError.empty());
    EXPECT_NE(receivedError.find("error"), std::string::npos);
}

TEST(AIAssistantMockTest, AskCalledExactlyOnce) {
    MockAIHealthAssistant mock;

    EXPECT_CALL(mock, ask(_, _)).Times(1)
        .WillOnce(Invoke([](const std::string &,
            std::function<void(const std::string&, const std::string&)> cb)
        { cb("response", ""); }));

    mock.ask("question", [](const std::string&, const std::string&) {});
}

TEST(AIAssistantMockTest, EmptyQuestionStillCallsCallback) {
    MockAIHealthAssistant mock;
    bool callbackCalled = false;

    EXPECT_CALL(mock, ask(_, _))
        .WillOnce(Invoke([](const std::string &,
            std::function<void(const std::string&, const std::string&)> cb)
        { cb("Please ask a question.", ""); }));

    mock.ask("", [&callbackCalled](const std::string &answer, const std::string &) {
        callbackCalled = !answer.empty();
    });

    EXPECT_TRUE(callbackCalled);
}
