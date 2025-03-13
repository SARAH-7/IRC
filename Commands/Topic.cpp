#include "Command.hpp"

void Command::execTopic()
{
    if (args.empty())
    {
        server.sendToClient(client.getFd(), RED "461: "  + client.getNick() + " TOPIC :Not enough parameters\r\n" RESET);
        return;
    }
    std::string channelName = args[0];
    Channel* channel = server.getChannel(channelName);
    if (!channel)
    {
        server.sendToClient(client.getFd(), RED "403: " + client.getNick() + " " + channelName + " :No such channel\r\n" RESET);
        return;
    }
    if(!channel->isMember(client.getFd()))
    {
        server.sendToClient(client.getFd(), " "  + client.getNick() + " " + channelName + " You are not in the channel\r\n");
        return;
    }
    if (args.size() == 1)
    {
        server.sendToClient(client.getFd(), "332: "  + client.getNick() + " " + channelName + " :" + channel->getTopic() + "\r\n");
        return;
    }
    if (channel->getMode('t') && !channel->isOperator(client.getFd()))
    {
        server.sendToClient(client.getFd(), RED "482: " + client.getNick() + " " + channelName + " :You're not a channel operator\r\n" RESET);
        return;
    }
    std::string topic;
    for (size_t i = 1; i < args.size(); i++)
    {
        if (i > 1)
            topic += " ";
        topic += args[i];
    }
    channel->setTopic(topic);
    server.sendToChannel(client.getFd(), channel->getMembers(), ":" + client.getNick() + " Topic " + channelName + " : " + topic + "\r\n");
}