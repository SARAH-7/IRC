#include "Command.hpp"

Command::Command(std::string buffer, Client &client, Server &server) : buffer(buffer), command(""), client(client), server(server) {}

Command::~Command() {}

//fix parsing for newline
void Command::parseBuffer()
{
    if(buffer.empty())
        return ;
    
    std::stringstream ss(this->buffer);
    ss >> this->command;

    std::string arg;
    while(ss >> arg)
        this->args.push_back(arg);
}

void Command::executeCommand()
{
    if (this->command == "PASS")
        execPass();
    else if (this->command == "USER")
        execUser();
    else if (this->command == "NICK")
        execNick();
    else if (this->command == "JOIN")
        execJoin();
    else if (this->command == "PART")
        execPart();
    else if (this->command == "NICK")
        execNick();
    else if (this->command == "TOPIC")
        execTopic();
    else if (this->command == "KICK")
        execKick();
    else if (this->command == "INVITE")
        execInvite();
    else if (this->command == "MODE")
        execMode();
    else if (command == "QUIT")
        execQuit();
    else if(command == "HELP")
        execHelp();
    else if (this->command == "PRIVMSG")
        execPrivMsg();
    else
        server.sendToClient(client.getFd(), RED "Unknown command: " + this->command + " If you need assisatnce use the HELP command\n" RESET);
}




