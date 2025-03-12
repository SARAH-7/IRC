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
    if(channelName[0] != '#')
    {
        server.sendToClient(client.getFd(), RED "476 " + channelName + " :Bad Channel Name\n" RESET);
        return;
    }
    if(args.size() > 1)
        password = args[1];
    Channel *channel = server.getChannel(channelName);
    if(!channel)
        channel = server.createChannel(channelName);
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
    if (channel->getMode('k'))
    {
        std::cout << "passowrd: " << password << "\n" << "channel " << channel->getPassword() << "\n";
        if (password.empty() || password != channel->getPassword())
        {
            server.sendToClient(client.getFd(), RED "475 " + channelName + " :Incorrect password\n" RESET);
            return;
        }
    }
    if (channel->getMode('l') && channel->getCurrentUsers() >= channel->getLimits())
    {
        server.sendToClient(client.getFd(), RED "471 " + channelName + " :Channel is full\n" RESET);
        return;
    }
    channel->removeInvitedUser(&client);
    channel->addMember(&client);
    server.sendToClient(client.getFd(), GREEN "Joined " + channelName + " successfully\n" RESET);
    if (channel->getMembers().size() == 1)
    {
        channel->giveOperatorPrivilage(&client);
        server.sendToClient(client.getFd(), CYAN "You are now the operator of " + channelName + "\n" RESET );
    }
    server.sendToChannel(channel->getMembers(), client.getNick() + " has joined " + channelName + "\n");
}