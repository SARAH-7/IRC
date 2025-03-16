#ifndef BOT_HPP
#define BOT_HPP

class Server;

#include "Client.hpp"

class Bot {
public:
    Bot(Server& server);
    void processMessage(Client* sender, const std::string& message);

private:
    Server& _server;
};

#endif
