#include "Command.hpp"

void Command::execPart()
{
    if(args.empty())
    {
        server.sendToClient(client.getFd(), RED "461 PART :Not enough parameters\n" RESET);
        return ;
    }
    std::string channelName = args[0];
    std::string reason;
    if (args.size() > 0)
    {
        for (size_t i = 1; i < args.size(); i++)
        {
            if (i > 1)
                reason += " ";
            reason += args[i];
        }
    }
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
    {
        server.deleteChannel(channelName);
        return ;
    }
    server.sendToChannel(channel->getMembers(), client.getNick() + " has left " + channelName + " because " + reason + "\n");
}