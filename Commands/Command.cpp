#include "Command.hpp"
#include "../Bot.hpp"

Command::Command(std::string buffer, Client &client, Server &server, Bot &bot) : buffer(buffer), command(""), client(client), server(server), bot(bot) {}

Command::~Command() {}

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
    else if(command == "NOTICE")
        execNotice();
    else if (this->command == "PRIVMSG")
        execPrivMsg();
    else if (this->command == "SENDFILE")
        execSendFile();
    else if (this->command[0] == '!')
    {
        if(args.size() == 0)
            bot.processMessage(&client, this->command);
        else
        {
            server.sendToClient(client.getFd(), RED "461: " + client.getNick() + " :!bot : Wrong number of parameters : !bot or !time or !help\r\n" RESET);
            return ;
        }
    }
    else
        server.sendToClient(client.getFd(), RED "Unknown command: " + this->command + "\n" + " If you need assisatnce use the HELP command\n" RESET);
}




