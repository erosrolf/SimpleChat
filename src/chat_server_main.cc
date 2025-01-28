#include <iostream>
#include "ws/ChatServer.h"

void serverRun(uint16_t port)
{
    ChatServer server;

    server.setOnOpenCallback([](connection_hdl hdl)
    {
        std::cout << "Client connected!" << std::endl;
    });

    server.setOnCloseCallback([](connection_hdl hdl)
    {
        std::cout << "Client disconnected!" << std::endl;
    });

    server.setOnMessageCallback([&server](connection_hdl hdl, const std::string& message)
    {
        std::cout << "Message from client: " << message << std::endl;
        server.send(hdl, "Echo: " + message, websocketpp::frame::opcode::text);
    });

    server.run(port);
}

int main(int argc, char* argv[])
{
    uint16_t port = 9004;

    if (argc > 1)
    {
        try
        {
            port = atoi(argv[1]);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Invalid port: " << argv[1] << std::endl;
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }

    try
    {
        serverRun(port);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
