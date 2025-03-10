#include "Command.hpp"

void Command::execPart()
{
    if(args.empty())
    {
        server.sendToClient(client.getFd(), RED "461 PART :Not enough parameters\n" RESET);
        return ;
    }
    std::string channelName = args[0];
    Channel* channel = server.getChannel(channelName);
    if (!channel)
    {
        server.sendToClient(client.getFd(), RED "403 " + channelName + " :No such channel\n" RESET);
        return;
    }
    if (!channel->isMember(client.getFd()))
    {
        server.sendToClient(client.getFd(), RED "442 " + channelName + " :You're not in this channel\n" RESET);
        return;
    }
    if (channel->isOperator(client.getFd()))
        channel->removeOperatorPrivilage(&client);
    channel->removeMember(&client);
    if(channel->getMembers().empty())
        server.deleteChannel(channelName);
    server.sendToChannel(client.getFd(), channel->getMembers(), client.getNick() + " has left " + channelName + "\n");
}