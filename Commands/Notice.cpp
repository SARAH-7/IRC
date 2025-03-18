#include "Command.hpp"

void Command::execNotice()
{
	if(!client.getIsAuthenticated() || !client.getIsRegistered())
    {
        server.sendToClient(client.getFd(), RED "451: " + client.getNick() + " :You have not registered\r\n" RESET);
        return ;
    }

    if (args.size() < 2)
    {
        server.sendToClient(client.getFd(), RED "461: " + client.getNick() + " NOTICE :Not enough parameters\r\n" RESET);
        return;
    }
    std::string target = args[0];
    std::string message;
    for (size_t i = 1; i < args.size(); i++)
    {
        message += args[i];
        if (i < args.size() - 1)
            message += " ";
    }
    std::string formattedMessage = ":" + client.getPrefix() + " NOTICE " + target + " :" + message + "\r\n";
    if (target[0] == '#')
    {
        Channel* channel = server.getChannel(target);
        if (!channel)
            return;
        server.sendToChannel(client.getFd(), channel->getMembers(), formattedMessage);
    }
    else
    {
        Client* recipient = server.getClientByNick(target);
        if (!recipient)
            return;
        server.sendToClient(recipient->getFd(), formattedMessage);
    }
}