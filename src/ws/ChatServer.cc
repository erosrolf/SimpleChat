#include "ChatServer.h"
#include <iostream>

ChatServer::ChatServer()
{
    server_.init_asio();

    server_.set_open_handler([this](connection_hdl hdl) { onOpen(hdl); });
    server_.set_close_handler([this](connection_hdl hdl) { onClose(hdl); });
    server_.set_message_handler([this](connection_hdl hdl, message_ptr msg) { onMessage(hdl, msg); });
}

void ChatServer::run(uint16_t port)
{
    server_.listen(port);
    server_.start_accept();
    std::cout << "Server is running on port " << port << std::endl;
    server_.run();
}

void ChatServer::send(connection_hdl hdl, std::string message, opcode op)
{
    absl::MutexLock lock(&connectionsMutex_);
    server_.send(hdl, message, op);
}

size_t ChatServer::connectionsCount() const
{
    absl::MutexLock lock(&connectionsMutex_);
    return connections_.size();
}

void ChatServer::closeAllConnections(websocketpp::close::status::value code, const std::string& reason)
{
    absl::MutexLock lock(&connectionsMutex_);
    for (const auto& connection : connections_)
    {
        server_.close(connection, code, reason);
    }
    connections_.clear();
}

void ChatServer::setOnMessageCallback(std::function<void(connection_hdl, const std::string&)> callback)
{
    onMessageCallback_ = std::move(callback);
}

void ChatServer::setOnOpenCallback(std::function<void(connection_hdl)> callback)
{
    onOpenCallback_ = std::move(callback);
}


void ChatServer::setOnCloseCallback(std::function<void(connection_hdl)> callback)
{
    onCloseCallback_ = std::move(callback);
}

void ChatServer::onOpen(connection_hdl hdl)
{
    absl::MutexLock lock(&connectionsMutex_);
    connections_.insert(hdl);

    if (onOpenCallback_)
    {
        onOpenCallback_(hdl);
    }

    std::cout << "New connection opened. Total connections: " << connections_.size() << std::endl;
}

void ChatServer::onClose(connection_hdl hdl)
{
    absl::MutexLock lock(&connectionsMutex_);
    connections_.erase(hdl);

    if (onCloseCallback_)
    {
        onCloseCallback_(hdl);
    }

    std::cout << "Connection closed. Total connections: " << connections_.size() << std::endl;
}

void ChatServer::onMessage(connection_hdl hdl, message_ptr msg)
{
    std::string message = msg->get_payload();

    if (onMessageCallback_)
    {
        onMessageCallback_(hdl, message);
    }

    std::cout << "Received message: " << message << std::endl;
}