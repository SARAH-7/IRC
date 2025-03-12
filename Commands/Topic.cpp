#include "Command.hpp"

void Command::execTopic()
{
    if (args.empty())
    {
        server.sendToClient(client.getFd(), RED "461 TOPIC :Not enough parameters\n" RESET);
        return;
    }
    std::string channelName = args[0];
    Channel* channel = server.getChannel(channelName);
    if (!channel)
    {
        server.sendToClient(client.getFd(), RED "403 " + channelName + " :No such channel\n" RESET);
        return;
    }
    if(!channel->isMember(client.getFd()))
    {
        server.sendToClient(client.getFd(), " " + channelName + " You are not in the channel\n");
        return;
    }
    if (args.size() == 1)
    {
        server.sendToClient(client.getFd(), "332 " + channelName + " :" + channel->getTopic() + "\n");
        return;
    }
    if (channel->getMode('t') && !channel->isOperator(client.getFd()))
    {
        server.sendToClient(client.getFd(), RED "482 " + channelName + " :You're not a channel operator\n" RESET);
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
    server.sendToChannel(channel->getMembers(), "Topic for " + channelName + " changed to: " + topic + "\n");
}