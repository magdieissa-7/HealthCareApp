#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <boost/asio.hpp>
#include <string>
#include <functional>
#include <thread>
#include <memory>

// NetworkServer uses Boost.Asio ASYNC I/O to accept and handle connections.
// It runs on a background thread and is non-blocking.
// Satisfies: "The application must use asynchronous I/O."
class NetworkServer {
public:
    explicit NetworkServer(unsigned short port);
    ~NetworkServer();

    // Start the async server. handler(message) -> response string.
    // Calls handler on each received message and sends back the returned string.
    void start(std::function<std::string(const std::string &)> handler);
    void stop();

private:
    void doAccept();
    void doRead(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
    void doWrite(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
                 const std::string &response);

    unsigned short port;
    std::function<std::string(const std::string &)> messageHandler;

    boost::asio::io_context   io_context;
    boost::asio::executor_work_guard<
        boost::asio::io_context::executor_type> workGuard;
    boost::asio::ip::tcp::acceptor acceptor;
    std::thread ioThread;
};

#endif // NETWORKSERVER_H
