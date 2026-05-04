#include "NetworkServer.h"
#include "JsonMessage.h"
#include <iostream>

using boost::asio::ip::tcp;

NetworkServer::NetworkServer(unsigned short port)
    : port(port),
      workGuard(boost::asio::make_work_guard(io_context)),
      acceptor(io_context, tcp::endpoint(tcp::v4(), port))
{}

NetworkServer::~NetworkServer() {
    stop();
}

void NetworkServer::start(std::function<std::string(const std::string &)> handler) {
    messageHandler = handler;
    doAccept();

    ioThread = std::thread([this]() { io_context.run(); });
    std::cout << "[NetworkServer] Listening on port " << port << "\n";
}

void NetworkServer::stop() {
    workGuard.reset();
    io_context.stop();
    if (ioThread.joinable()) ioThread.join();
}



void NetworkServer::doAccept() {
    auto socket = std::make_shared<tcp::socket>(io_context);
    acceptor.async_accept(*socket,
        [this, socket](const boost::system::error_code &ec)
    {
        if (!ec) {
            doRead(socket);
        } else if (ec != boost::asio::error::operation_aborted) {
            std::cerr << "[NetworkServer] Accept error: " << ec.message() << "\n";
        }

        if (!io_context.stopped()) doAccept();
    });
}



void NetworkServer::doRead(std::shared_ptr<tcp::socket> socket) {
    auto buf = std::make_shared<boost::asio::streambuf>();
    boost::asio::async_read_until(*socket, *buf, '\n',
        [this, socket, buf](const boost::system::error_code &ec, std::size_t)
    {
        if (ec) {

            if (ec == boost::asio::error::eof)
                std::cout << "[NetworkServer] Client disconnected.\n";
            else
                std::cerr << "[NetworkServer] Read error: " << ec.message() << "\n";
            return;
        }

        std::istream is(buf.get());
        std::string message;
        std::getline(is, message);


        if (!JsonMessage::isValidMessage(message)) {
            std::cerr << "[NetworkServer] Invalid message received: " << message << "\n";
            doWrite(socket, JsonMessage::createAckMessage("error: invalid message"));
            return;
        }

        std::string response = messageHandler ? messageHandler(message)
                                              : JsonMessage::createAckMessage("ok");
        doWrite(socket, response);
    });
}



void NetworkServer::doWrite(std::shared_ptr<tcp::socket> socket,
                             const std::string &response)
{
    auto buf = std::make_shared<std::string>(response + "\n");
    boost::asio::async_write(*socket, boost::asio::buffer(*buf),
        [socket, buf](const boost::system::error_code &ec, std::size_t)
    {
        if (ec)
            std::cerr << "[NetworkServer] Write error: " << ec.message() << "\n";
    });
}
