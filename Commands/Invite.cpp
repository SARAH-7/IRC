#include "Command.hpp"

void Command::execInvite()
{
    if(args.size() < 2)
    {
        server.sendToClient(client.getFd(), RED "461 INVITE :Not enough parameters\n" RESET);
        return;
    }
    std::string clientNick = args[0];
    std::string channelName = args[1];

    Channel *channel = server.getChannel(channelName);
    if(!channel)
    {
        server.sendToClient(client.getFd(), RED "403 " + channelName + " :No such channel\n" RESET);
        return;
    }
    if (!channel->isOperator(client.getFd()))
    {
        server.sendToClient(client.getFd(), RED "482 " + channelName + " :You're not a channel operator\n" RESET);
        return;
    }
    Client* invitedClient = server.getClientByNick(clientNick);
    if (!invitedClient)
    {
        server.sendToClient(client.getFd(), RED "401 " + clientNick + " :No such nick\n" RESET);
        return;
    }
    channel->addInvitedUser(invitedClient);
    server.sendToClient(invitedClient->getFd(), YELLOW "You have been invited to " + channelName + "\n" RESET);
    server.sendToClient(client.getFd(), GREEN "You have successfully invited " + clientNick + " to " + channelName + "\n" RESET);
}