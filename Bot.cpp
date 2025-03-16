#include "Bot.hpp"
#include "Server.hpp"
#include <ctime>
#include <iostream>

Bot::Bot(Server& server) : _server(server) {}

void Bot::processMessage(Client* sender, const std::string& message) {
    if (message == "!hello") {
        _server.sendToClient(sender->getFd(), "Bot: Hello, " + sender->getNick() + "! if you want to know the current time enter !time, if you need help enter !help\n");
    } 
    else if (message == "!time") {
        time_t now = time(0);
        _server.sendToClient(sender->getFd(), "Bot: The current time is " + std::string(ctime(&now)));
    }
    else if (message == "!help") {
        _server.sendToClient(sender->getFd(), "Bot: Available commands: !hello, !time, !help\n");
    }
}
