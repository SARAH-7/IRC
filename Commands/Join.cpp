#include "Command.hpp"

void Command::execJoin()
{
	if(!client.getIsAuthenticated() || !client.getIsRegistered())
    {
        server.sendToClient(client.getFd(), RED "451: " + client.getNick() + " :You have not registered\r\n" RESET);
        return ;
    }

    if(args.empty())
    {
        server.sendToClient(client.getFd(), RED "461: " + client.getNick() + " JOIN :Not enough parameters\r\n" RESET);
        return ;
    }
    std::string channelName = args[0];
    std::string password = "";
    if (channelName[0] != '#') 
    {
        server.sendToClient(client.getFd(), RED "476: " + client.getNick() + " " + channelName + " :Bad Channel Mask\r\n" RESET);
        return;
    }
    if(args.size() > 2)
    {
        server.sendToClient(client.getFd(), RED "461: " + client.getNick() + " :JOIN : Wrong number of parameters : JOIN <channel> optional : <passowrd>\r\n" RESET);
        return ;
    }
    Channel *channel = server.getChannel(channelName);
    if(!channel)
        channel = server.createChannel(channelName);
    if(args.size() > 1)
    {
        if(channel->getMode('k'))
            password = args[1];
        else
        {
            server.sendToClient(client.getFd(), RED "461: " + client.getNick() + " :JOIN : Wrong number of parameters. JOIN <channel>\r\n" RESET);
            return ;
        }
    }
    if (channel->isMember(client.getFd()))
    {
        server.sendToClient(client.getFd(), RED "442: " + client.getNick() + " " + channelName + " :You're are already in this channel\r\n" RESET);
        return;
    }
    if (channel->getMode('i') && !channel->isInvited(&client)) 
    {
        server.sendToClient(client.getFd(), RED "473: " + client.getNick() + " " + channelName + " :Cannot join channel (channel is invite only)\r\n" RESET);
        return;
    }
    if (channel->getMode('k'))
    {
        if (password.empty() || password != channel->getPassword())
        {
            server.sendToClient(client.getFd(), RED "475: " + client.getNick() + " " + channelName + " :Cannot join channel (channel is password locked)\r\n" RESET);
            return;
        }
    }
    if (channel->getMode('l') && channel->getCurrentUsers() >= channel->getLimits())
    {
        server.sendToClient(client.getFd(), RED "471: " + client.getNick() + " " + channelName + " :Cannot join channel (Channel is full)\r\n" RESET);
        return;
    }
    channel->removeInvitedUser(&client);
    channel->addMember(&client);
    std::string joinMsg =  ":" + client.getPrefix() + " JOIN " + channelName + "\r\n";
    server.sendToChannel(client.getFd(), channel->getMembers(), joinMsg);
    if (channel->getMembers().size() == 1)
        channel->giveOperatorPrivilage(&client);
    std::vector<Client *> members = channel->getMembers();
    std::string memberList;
    for (size_t i = 0; i < members.size(); i++)
    {
        memberList += members[i]->getNick();
        if (i != members.size() - 1)  
            memberList += " ";
    }
    server.sendToClient(client.getFd(), "332: " + client.getNick() + " " + channelName + " :" + channel->getTopic() + "\r\n");
    server.sendToClient(client.getFd(), "353: " + client.getNick() + " = " + channelName + " :" + memberList + "\r\n");
    server.sendToClient(client.getFd(), "366: " + client.getNick() + " " + channelName + " :End of NAMES list\r\n");
}