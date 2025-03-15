#include "Command.hpp"

void Command::execUser()
{
    if(!client.getIsAuthenticated())
    {
        server.sendToClient(client.getFd(),RED "451: :You have not registered\r\n" RESET);
        return ;
    }
    if(client.getIsRegistered() || !client.getUser().empty())
    {
        server.sendToClient(client.getFd(), RED "462: You may not reregister\r\n" RESET);
        return ;
    }
    if(args.size() < 4 || args.empty())
    {
        server.sendToClient(client.getFd(), RED "461: Not enough parameters\r\n" RESET);
        return ;
    }
    client.setUser(args[0]);
    client.setHost(args[1]);
    client.setRealName(args[3]);
    if(!client.getNick().empty())
    {
        client.setIsRegistered(true);
        server.sendToClient(client.getFd(), "001 " + client.getNick() + " :Welcome to the Internet Relay Netwrok, " + client.getPrefix() + "\r\n");
    }
}