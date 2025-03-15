#include "Command.hpp"

void Command::execKick()
{
    if (args.size() < 2)
    {
        server.sendToClient(client.getFd(), RED "461: " + client.getNick() + " KICK :Not enough parameters\r\n" RESET);
        return;
    }
    std::string channelName = args[0];
    std::string nick = args[1];
    Channel* channel = server.getChannel(channelName);
    if (!channel)
    {
        server.sendToClient(client.getFd(), RED "403: " + client.getNick() + " " + channelName + " :No such channel\r\n" RESET);
        return;
    }
    if (!channel->isOperator(client.getFd()))
    {
        server.sendToClient(client.getFd(), RED "482: " + client.getNick() + " " + channelName + " :You're not a channel operator\r\n" RESET);
        return;
    }
    Client *kickClient = server.getClientByNick(nick);
    if(!kickClient || !channel->isMember(kickClient->getFd()))
    {
        server.sendToClient(client.getFd(), RED "441: " + client.getNick() + nick + " " + channelName + " :They aren't in the channel\r\n" RESET);
        return ;
    }
    std::string kickMsg = ":" + client.getPrefix() + " KICK " + channelName + " " + nick + " :Kicked by " + client.getNick() + "\r\n";
    
    channel->removeMember(kickClient);
    server.sendToChannel(client.getFd(), channel->getMembers(), kickMsg);
    server.sendToClient(kickClient->getFd(), kickClient->getNick() + " :You have been kicked out of " + channelName + "\r\n");
}