#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include "INetworkClient.h"
#include <boost/asio.hpp>
#include <string>
#include <functional>
#include <thread>
#include <memory>

// NetworkClient implements INetworkClient using Boost.Asio ASYNC I/O.
// All operations are non-blocking: results are delivered via callbacks.
// This satisfies the requirement: "The application must use asynchronous I/O."
class NetworkClient : public INetworkClient {
public:
    NetworkClient(const std::string &host, const std::string &port);
    ~NetworkClient() override;

    // Asynchronously sends a message and calls the callback when done.
    // callback(response, error) – error is empty on success.
    void sendMessage(const std::string &message,
                     std::function<void(const std::string &response,
                                        const std::string &error)> callback) override;

    void disconnect() override;

private:
    void doConnect(const std::string &message,
                   std::function<void(const std::string&, const std::string&)> callback);
    void doWrite(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
                 const std::string &message,
                 std::function<void(const std::string&, const std::string&)> callback);
    void doRead(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
                std::shared_ptr<boost::asio::streambuf> buf,
                std::function<void(const std::string&, const std::string&)> callback);

    std::string host;
    std::string port;

    boost::asio::io_context              io_context;
    boost::asio::executor_work_guard<
        boost::asio::io_context::executor_type> workGuard;
    std::thread                          ioThread;  // runs io_context in background
};

#endif // NETWORKCLIENT_H
