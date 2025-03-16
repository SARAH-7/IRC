#include "Command.hpp"

void Command::execInvite()
{
	if(!client.getIsAuthenticated() || !client.getIsRegistered())
    {
        server.sendToClient(client.getFd(), RED "451: " + client.getNick() + " :You have not registered\r\n" RESET);
        return ;
    }
	if(args.size() < 2)
    {
        server.sendToClient(client.getFd(), RED ":461 " + client.getNick() + " INVITE :Not enough parameters\r\n" RESET);
        return;
    }
    std::string clientNick = args[0];
    std::string channelName = args[1];

    Channel *channel = server.getChannel(channelName);
    if(!channel)
    {
        server.sendToClient(client.getFd(), RED ":403 " + client.getNick() + " " + channelName + " :No such channel\r\n" RESET);
        return;
    }
    if (!channel->isOperator(client.getFd()))
    {
        server.sendToClient(client.getFd(), RED ":482 " + client.getNick() + " " + channelName + " :You're not a channel operator\r\n" RESET);
        return;
    }
    Client* invitedClient = server.getClientByNick(clientNick);
    if (!invitedClient)
    {
        server.sendToClient(client.getFd(), RED ":401 " + client.getNick() + " :No such nick\n" RESET);
        return;
    }
    channel->addInvitedUser(invitedClient);
    server.sendToClient(invitedClient->getFd(), YELLOW "You have been invited to " + channelName + "\r\n" RESET);
    server.sendToChannel(client.getFd(), channel->getMembers(), GREEN ": " + client.getNick() + " successfully invited " + clientNick + " to " + channelName + "\r\n" RESET);
}