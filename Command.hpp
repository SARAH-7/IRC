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

class Client;

class Command{
    private:
    std::vector<std::string> args;
    std::string buffer;
    std::vector<std::string> nicknameStored;
    std::string command;
    
    Client &client;

    public:
    Command(std::string buffer, Client &client);
    ~Command();

    void parseBuffer();
    void executeCommand( void );
    void execUser();
    void execNick();
};

#endif