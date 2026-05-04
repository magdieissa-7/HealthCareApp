#ifndef INETWORK_CLIENT_H
#define INETWORK_CLIENT_H
#include <string>
#include <functional>

// Abstract interface for the network client.
// Having this interface allows GoogleMock to create a mock
// version for unit testing without needing a real server.
class INetworkClient {
public:
    virtual ~INetworkClient() = default;
    virtual void sendMessage(const std::string &message,
                             std::function<void(const std::string &response,
                                                const std::string &error)> callback) = 0;
    virtual void disconnect() = 0;
};

#endif // INETWORK_CLIENT_H
