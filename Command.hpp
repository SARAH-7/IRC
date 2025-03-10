#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <exception>
#include <vector>
#include <cstring>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include "../Client.hpp"
#include "../Server.hpp"

class Client;
class Server;

class Command {
private:
    std::vector<std::string> args;
    std::string buffer;
    std::vector<std::string> nicknameStored;
    std::string command;
    
    Client &client;
    Server &server;

public:
    Command(std::string buffer, Client &client, Server &server);
    ~Command();

    void parseBuffer();
    void executeCommand();
    void execUser();
    void execNick();
    void execPrivMsg(); // New method to handle PRIVMSG command
};

#endif