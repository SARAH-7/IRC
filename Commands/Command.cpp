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
        bot.processMessage(&client, this->command);
    else
        server.sendToClient(client.getFd(), RED "Unknown command: " + this->command + " If you need assisatnce use the HELP command\n" RESET);
}

// void Command::executeCommand()
// {
//     if (this->command == "PASS")
//         execPass();
//     else if (this->command == "USER")
//         execUser();
//     else if (this->command == "NICK")
//         execNick();
//     else if (this->command == "JOIN")
//         execJoin();
//     else if (this->command == "PART")
//         execPart();
//     else if (this->command == "NICK")
//         execNick();
//     else if (this->command == "TOPIC")
//         execTopic();
//     else if (this->command == "KICK")
//         execKick();
//     else if (this->command == "INVITE")
//         execInvite();
//     else if (this->command == "MODE")
//         execMode();
//     else if (command == "QUIT")
//         execQuit();
//     else if(command == "HELP")
//         execHelp();
//     else if(command == "NOTICE")
//         execNotice();
//     else if (this->command == "PRIVMSG")
//         execPrivMsg();
//     else if (this->command == "SENDFILE") {
//         if (args.size() < 3) {
//             _server.sendToClient(_client.getFd(), "Usage: /SENDFILE <filename> <recipient>\n");
//             return;
//         }
//         std::string filename = args[1];
//         std::string recipient = args[2];
//         FileTransfer fileTransfer(_server);
//         fileTransfer.sendFile(&_client, filename, recipient);
//     }
//     else
//         server.sendToClient(client.getFd(), RED "Unknown command: " + this->command + " If you need assisatnce use the HELP command\n" RESET);
// }




