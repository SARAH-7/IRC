#include "Command.hpp"

void Command::execKick()
{
    if (args.size() < 2)
    {
        server.sendToClient(client.getFd(), RED "461 KICK :Not enough parameters\n" RESET);
        return;
    }
    std::string channelName = args[0];
    std::string nick = args[1];
    Channel* channel = server.getChannel(channelName);
    if (!channel)
    {
        server.sendToClient(client.getFd(), RED "403 " + channelName + " :No such channel\n" RESET);
        return;
    }
    if (!channel->isOperator(client.getFd()))
    {
        server.sendToClient(client.getFd(), RED "482 " + channelName + " :You're not a channel operator\n" RESET);
        return;
    }
    Client *kickClient = server.getClientByNick(nick);
    if(!kickClient || !channel->isMember(kickClient->getFd()))
    {
        server.sendToClient(client.getFd(), RED "441 " + nick + " " + channelName + " :They aren't in the channel\n" RESET);
        return ;
    }
    channel->removeMember(kickClient);
    server.sendToChannel(client.getFd(), channel->getMembers(), nick + " has been kicked from " + channelName + "\n");
}