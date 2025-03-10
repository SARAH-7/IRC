#include "Command.hpp"

void Command::execHelp()
{
    std::string response = "Available commands:\n";
    response += "JOIN <#channel> - Join a channel\n";
    response += "PART <#channel> - Leave a channel\n";
    response += "QUIT - Disconnect from the server\n";
    response += "INVITE <user> <#channel> - Invite a user to a channel\n";
    response += "TOPIC <#channel> [topic] - Change or view the channel topic\n";
    response += "KICK <#channel> <user> - Remove a user from a channel\n";
    response += "MODE <#channel> <mode> - Change channel settings\n";

    server.sendToClient(client.getFd(), response);
}
