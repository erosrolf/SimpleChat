#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <functional>
#include <string>

using connection_hdl = websocketpp::connection_hdl;

class IServerEndpoint {
public:
    using message_ptr = websocketpp::config::asio::message_type::ptr;
    using opcode = websocketpp::frame::opcode::value;

    virtual ~IServerEndpoint() = default;

    virtual void run(uint16_t port) = 0;

    virtual void send(connection_hdl hdl, std::string message, opcode op) = 0;

    virtual size_t connectionsCount() const = 0;

    virtual void closeAllConnections(websocketpp::close::status::value code, const std::string& reason) = 0;

    virtual void setOnMessageCallback(std::function<void(connection_hdl, const std::string&)> callback) = 0;

    virtual void setOnOpenCallback(std::function<void(connection_hdl)> callback) = 0;

    virtual void setOnCloseCallback(std::function<void(connection_hdl)> callback) = 0;
};
