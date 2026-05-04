#include "NetworkClient.h"
#include <iostream>
#include <memory>

using boost::asio::ip::tcp;

NetworkClient::NetworkClient(const std::string &host, const std::string &port)
    : host(host), port(port),
      workGuard(boost::asio::make_work_guard(io_context))
{

    ioThread = std::thread([this]() { io_context.run(); });
}

NetworkClient::~NetworkClient() {
    disconnect();
}

void NetworkClient::disconnect() {
    workGuard.reset();
    io_context.stop();
    if (ioThread.joinable()) ioThread.join();
}


void NetworkClient::sendMessage(
    const std::string &message,
    std::function<void(const std::string &, const std::string &)> callback)
{

    boost::asio::post(io_context, [this, message, callback]() {
        doConnect(message, callback);
    });
}



void NetworkClient::doConnect(
    const std::string &message,
    std::function<void(const std::string&, const std::string&)> callback)
{
    auto socket = std::make_shared<tcp::socket>(io_context);
    auto resolver = std::make_shared<tcp::resolver>(io_context);

    resolver->async_resolve(host, port,
        [this, socket, resolver, message, callback]
        (const boost::system::error_code &ec, tcp::resolver::results_type endpoints)
    {
        if (ec) {
            callback("", "Resolve error: " + ec.message());
            return;
        }

        boost::asio::async_connect(*socket, endpoints,
            [this, socket, message, callback]
            (const boost::system::error_code &ec, const tcp::endpoint &)
        {
            if (ec) {
                callback("", "Connect error: " + ec.message());
                return;
            }
            doWrite(socket, message, callback);
        });
    });
}

void NetworkClient::doWrite(
    std::shared_ptr<tcp::socket> socket,
    const std::string &message,
    std::function<void(const std::string&, const std::string&)> callback)
{
    auto buf = std::make_shared<std::string>(message + "\n");

    boost::asio::async_write(*socket, boost::asio::buffer(*buf),
        [this, socket, buf, callback]
        (const boost::system::error_code &ec, std::size_t)
    {
        if (ec) {
            callback("", "Write error: " + ec.message());
            return;
        }
        auto readBuf = std::make_shared<boost::asio::streambuf>();
        doRead(socket, readBuf, callback);
    });
}

void NetworkClient::doRead(
    std::shared_ptr<tcp::socket> socket,
    std::shared_ptr<boost::asio::streambuf> buf,
    std::function<void(const std::string&, const std::string&)> callback)
{
    // Read until newline delimiter
    boost::asio::async_read_until(*socket, *buf, '\n',
        [socket, buf, callback]
        (const boost::system::error_code &ec, std::size_t)
    {
        if (ec && ec != boost::asio::error::eof) {
            callback("", "Read error: " + ec.message());
            return;
        }
        std::istream is(buf.get());
        std::string response;
        std::getline(is, response);
        callback(response, "");   // success
    });
}
