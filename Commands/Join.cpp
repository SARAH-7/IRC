#include "Command.hpp"

void Command::execJoin()
{
    if(args.empty())
    {
        server.sendToClient(client.getFd(), RED "461 JOIN :Not enough parameters\n" RESET);
        return ;
    }
    std::string channelName = args[0];
    std::string password = "";
    if(args.size() > 1)
        password = args[1];
    Channel *channel = server.getChannel(channelName);
    if(!channel)
    {
        channel = server.createChannel(channelName);
        channel->giveOperatorPrivilage(&client);
    }
    if (channel->isMember(client.getFd()))
    {
        server.sendToClient(client.getFd(), RED "442 " + channelName + " :You're are already in this channel\n" RESET);
        return;
    }
    if (channel->getMode('i') && !channel->isInvited(&client)) 
    {
        server.sendToClient(client.getFd(), RED "473 " + channelName + " :Invite-only channel\n" RESET);
        return;
    }
    if (channel->getMode('k') && password != channel->getPassword())
    {
        server.sendToClient(client.getFd(), RED "475 " + channelName + " :Incorrect password\n" RESET);
        return;
    }
    if (channel->getMode('l') && channel->getCurrentUsers() >= channel->getLimits())
    {
        server.sendToClient(client.getFd(), RED "471 " + channelName + " :Channel is full\n" RESET);
        return;
    }
    channel->removeInvitedUser(&client);
    channel->addMember(&client);
    server.sendToClient(client.getFd(), GREEN "Joined " + channelName + " successfully\n" RESET);
    server.sendToChannel(client.getFd(), channel->getMembers(), client.getNick() + " has joined " + channelName + "\n");
}