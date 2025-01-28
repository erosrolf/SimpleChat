#pragma once

#include "IServerEndpoint.h"
#include "absl/synchronization/mutex.h"
#include <set>

class ChatServer : public IServerEndpoint {
public:
    ChatServer();
    ~ChatServer() override = default;

    void run(uint16_t port) override;

    void send(connection_hdl hdl, std::string message, opcode op) override;

    size_t connectionsCount() const override;

    void closeAllConnections(websocketpp::close::status::value code, const std::string& reason) override;

    void setOnMessageCallback(std::function<void(connection_hdl, const std::string&)> callback) override;

    void setOnOpenCallback(std::function<void(connection_hdl)> callback) override;

    void setOnCloseCallback(std::function<void(connection_hdl)> callback) override;

private:
    void onOpen(connection_hdl hdl);
    void onClose(connection_hdl hdl);
    void onMessage(connection_hdl hdl, message_ptr msg);

    websocketpp::server<websocketpp::config::asio> server_;
    std::set<connection_hdl, std::owner_less<connection_hdl>> connections_;
    mutable absl::Mutex connectionsMutex_;

    std::function<void(connection_hdl, const std::string&)> onMessageCallback_;
    std::function<void(connection_hdl)> onOpenCallback_;
    std::function<void(connection_hdl)> onCloseCallback_;
};