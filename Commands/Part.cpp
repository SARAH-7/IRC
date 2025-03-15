#include "Command.hpp"

void Command::execPart()
{
    if(args.empty())
    {
        server.sendToClient(client.getFd(), RED "461: "  + client.getNick() + " PART :Not enough parameters\r\n" RESET);
        return ;
    }
    std::string channelName = args[0];
    Channel* channel = server.getChannel(channelName);
    if (!channel)
    {
        server.sendToClient(client.getFd(), RED "403: "  + client.getNick() + " " + channelName + " :No such channel\r\n" RESET);
        return;
    }
    if (!channel->isMember(client.getFd()))
    {
        server.sendToClient(client.getFd(), RED "442: "  + client.getNick() + " " + channelName + " :You're not in this channel\r\n" RESET);
        return;
    }
    std::string partMsg = ":" + client.getPrefix() + " PART " + channelName + "\r\n";
    server.sendToChannel(client.getFd(), channel->getMembers(), partMsg);
    bool isOperator = channel->isOperator(client.getFd());
    if (channel->isOperator(client.getFd()))
        channel->removeOperatorPrivilage(&client);
    channel->removeMember(&client);
    if(channel->getMembers().empty())
    {
        server.deleteChannel(channelName);
        return ;
    }
    if (isOperator && channel->getOperators().empty())
    {
        std::vector<Client *> members = channel->getMembers();
        if (!members.empty())
        {
            channel->giveOperatorPrivilage(members[0]);
            server.sendToClient(members[0]->getFd(), CYAN "You are now the operator of " + channelName + "\r\n" RESET);
        }
    }
}