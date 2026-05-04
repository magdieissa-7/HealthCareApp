#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/network/INetworkClient.h"
#include "../src/network/JsonMessage.h"
#include "../src/core/ReminderManager.h"
#include "../src/core/Medication.h"
#include "../src/core/EmergencyManager.h"
#include "../src/core/AuthManager.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

// ── GoogleMock: mock the network interface ────────────────────────────────────
// This lets us test business logic WITHOUT a real server running.
class MockNetworkClient : public INetworkClient {
public:
    MOCK_METHOD(void, sendMessage,
        (const std::string &message,
         std::function<void(const std::string &, const std::string &)> callback),
        (override));
    MOCK_METHOD(void, disconnect, (), (override));
};

// ── Tests using the mock ──────────────────────────────────────────────────────

TEST(NetworkMockTest, SendMessageIsCalledWithLoginJson) {
    MockNetworkClient mockClient;

    std::string capturedMessage;
    // Expect sendMessage to be called once; capture what was sent
    EXPECT_CALL(mockClient, sendMessage(_, _))
        .Times(1)
        .WillOnce(Invoke([&capturedMessage](
            const std::string &msg,
            std::function<void(const std::string&, const std::string&)> cb)
        {
            capturedMessage = msg;
            cb("response", "");  // simulate server reply
        }));

    std::string loginMsg = JsonMessage::createLoginMessage("alice");
    mockClient.sendMessage(loginMsg, [](const std::string &, const std::string &) {});

    EXPECT_NE(capturedMessage.find("login"),    std::string::npos);
    EXPECT_NE(capturedMessage.find("alice"),    std::string::npos);
}

TEST(NetworkMockTest, SendMessageIsCalledWithEmergencyJson) {
    MockNetworkClient mockClient;

    EXPECT_CALL(mockClient, sendMessage(_, _))
        .Times(1)
        .WillOnce(Invoke([](
            const std::string &,
            std::function<void(const std::string&, const std::string&)> cb)
        {
            cb("", "");
        }));

    std::string msg = JsonMessage::createEmergencyMessage("01012345678");
    mockClient.sendMessage(msg, [](const std::string &, const std::string &) {});
}

TEST(NetworkMockTest, ServerErrorCallbackReceivesError) {
    MockNetworkClient mockClient;

    std::string receivedError;
    EXPECT_CALL(mockClient, sendMessage(_, _))
        .WillOnce(Invoke([](
            const std::string &,
            std::function<void(const std::string&, const std::string&)> cb)
        {
            cb("", "Connect error: Connection refused");  // simulate server down
        }));

    mockClient.sendMessage("test", [&receivedError](
        const std::string &, const std::string &error) {
        receivedError = error;
    });

    EXPECT_FALSE(receivedError.empty());
    EXPECT_NE(receivedError.find("error"), std::string::npos);
}

TEST(NetworkMockTest, DisconnectIsCalled) {
    MockNetworkClient mockClient;
    EXPECT_CALL(mockClient, disconnect()).Times(1);
    mockClient.disconnect();
}

TEST(NetworkMockTest, DISABLED_MedicationMessageSentAfterAddingMedication) {
    MockNetworkClient mockClient;
    ReminderManager rm;
    rm.addMedication(Medication("Aspirin", "100mg", "08:00"));

    std::string capturedMsg;
    EXPECT_CALL(mockClient, sendMessage(_, _))
        .WillOnce(Invoke([&capturedMsg](
            const std::string &msg,
            std::function<void(const std::string&, const std::string&)> cb)
        {
            capturedMsg = msg;
            cb("", "");
        }));

    const auto &med = rm.getMedications().back();
    std::string msg = JsonMessage::createMedicationMessage(
        med.getName(), med.getDosage(), med.getTime());
    mockClient.sendMessage(msg, [](const std::string&, const std::string&) {});

    EXPECT_NE(capturedMsg.find("Aspirin"), std::string::npos);
    EXPECT_NE(capturedMsg.find("100mg"),   std::string::npos);
}

// ── JSON Deserialization tests ────────────────────────────────────────────────

TEST(JsonDeserializeTest, ParseLoginMessage) {
    std::string json = JsonMessage::createLoginMessage("bob");
    auto fields = JsonMessage::parse(json);
    EXPECT_EQ(fields["type"],     "login");
    EXPECT_EQ(fields["username"], "bob");
}

TEST(JsonDeserializeTest, ParseEmergencyMessage) {
    std::string json = JsonMessage::createEmergencyMessage("01099999999");
    auto fields = JsonMessage::parse(json);
    EXPECT_EQ(fields["type"],    "emergency");
    EXPECT_EQ(fields["contact"], "01099999999");
}

TEST(JsonDeserializeTest, ParseMedicationMessage) {
    std::string json = JsonMessage::createMedicationMessage("Insulin", "10mg", "07:00");
    auto fields = JsonMessage::parse(json);
    EXPECT_EQ(fields["type"],   "medication");
    EXPECT_EQ(fields["name"],   "Insulin");
    EXPECT_EQ(fields["dosage"], "10mg");
    EXPECT_EQ(fields["time"],   "07:00");
}

TEST(JsonDeserializeTest, GetFieldExtractsCorrectly) {
    std::string json = JsonMessage::createAckMessage("ok");
    EXPECT_EQ(JsonMessage::getField(json, "type"),   "ack");
    EXPECT_EQ(JsonMessage::getField(json, "status"), "ok");
}

TEST(JsonDeserializeTest, InvalidJsonReturnsFalse) {
    EXPECT_FALSE(JsonMessage::isValidMessage(""));
    EXPECT_FALSE(JsonMessage::isValidMessage("not json"));
    EXPECT_FALSE(JsonMessage::isValidMessage("{}"));
}

TEST(JsonDeserializeTest, ValidMessageReturnsTrue) {
    EXPECT_TRUE(JsonMessage::isValidMessage(
        JsonMessage::createLoginMessage("alice")));
}

// ── AuthManager tests ─────────────────────────────────────────────────────────

TEST(AuthManagerTest, ValidCredentialsReturnTrue) {
    AuthManager auth;
    EXPECT_TRUE(auth.authenticate("admin", "1234"));
}

TEST(AuthManagerTest, WrongPasswordReturnsFalse) {
    AuthManager auth;
    EXPECT_FALSE(auth.authenticate("admin", "wrong"));
}

TEST(AuthManagerTest, WrongUsernameReturnsFalse) {
    AuthManager auth;
    EXPECT_FALSE(auth.authenticate("hacker", "1234"));
}
