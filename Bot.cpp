#include "Bot.hpp"
#include "Server.hpp"
#include <ctime>
#include <iostream>

Bot::Bot(Server& server) : _server(server) {}

void Bot::processMessage(Client* sender, const std::string& message) {
    if (message == "!bot") {
        _server.sendToClient(sender->getFd(), "Bot: Hello, " + sender->getNick() + "! if you want to know the current time enter !time, if you need help enter !help\n");
    } 
    else if (message == "!time") {
        time_t now = time(0);
        _server.sendToClient(sender->getFd(), "Bot: The current time is " + std::string(ctime(&now)));
    }
    else if (message == "!help") {
        _server.sendToClient(sender->getFd(), "Bot: Available commands: !hello, !time, !help\n");
    }
    else if (message == "!fact") {
        std::vector<std::string> facts = { "This is the best IRC!", "ChatOasis is the best server." };
        _server.sendToClient(sender->getFd(), "Bot: " + facts[rand() % facts.size()] + "\n");
    }
    else if (message == "!flip") {
        std::string result = (rand() % 2) ? "Heads" : "Tails";
        _server.sendToClient(sender->getFd(), "Bot: The coin landed on " + result + "!\n");
    }
    else if (message == "!ping") {
        _server.sendToClient(sender->getFd(), "Bot: Pong!\n");
    }
}
