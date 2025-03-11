#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <exception>
#include <vector>
#include <cstring>
#include <string>
#include <sstream>
#include <map>
#include <numeric>
#include <algorithm>
#include "../Client.hpp"
#include "../Server.hpp"

class Client;

class Server;

class Command{
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
    void executeCommand( void );
    void execUser();
    void execNick();
    void execJoin();
    void execPass();
    void execPart();
    void execTopic();
    void execKick();
    void execInvite();
    void execMode();
    void execQuit();
    void execHelp();
	void execPrivMsg();
    void applyMode(Channel *channel, char mode, bool enable, const std::string &modeType);
};

#endif