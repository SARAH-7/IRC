#include "Command.hpp"

bool isValidNick(const std::string &nick)
{
    if(nick.empty())
        return (false);
    if(nick[0] == '&' || nick[0] == '#' || nick[0] == ':')
        return (false);
    for(size_t i = 0; i < nick.length(); i++)
    {
        if(!std::isalnum(nick[i]) && nick[i] != '-')
            return (false);
    }
    return (true); 
}

void Command::execNick()
{
    if(!client.getIsAuthenticated())
    {
        server.sendToClient(client.getFd(), RED "451: " + client.getNick() + " :You have not registered\r\n" RESET);
        return ;
    }
    if(client.getIsRegistered() || !client.getNick().empty())
    {
        server.sendToClient(client.getFd(), RED "431: " + client.getNick() + " :You may not reregister\r\n" RESET);
        return ;
    }
    if(args.empty())
    {
        server.sendToClient(client.getFd(), RED "431: :No nickname given\r\n" RESET);
        return ;
    }
    if(args.size() > 1)
    {
        server.sendToClient(client.getFd(), RED "461: " + client.getNick() + " :NICK : Wrong number of parameters : NICK <nickname>\r\n" RESET);
        return ;
    }
    if(!isValidNick(args[0]))
    {
        server.sendToClient(client.getFd(), RED "432: " + args[0] + " :Erroneus nickname\r\n" RESET);
        return ; 
    }
    if (server.isNickInUse(args[0]))
    {
        server.sendToClient(client.getFd(), RED "433: " + args[0] + " :Nickname is already in use\r\n" RESET);
        return;
    }
    if (!client.getNick().empty())
        server.removeNick(client.getNick());
    client.setNick(args[0]);
    server.addNick(args[0]);
    std::string successMsg = "Your nickname has been set to " + client.getNick() + "\n";
    server.sendToClient(client.getFd(), successMsg);
    if(!client.getUser().empty())
    {
        client.setIsRegistered(true);
        server.sendToClient(client.getFd(), "001 " + client.getNick() + " :Welcome to the Internet Relay Network, " + client.getPrefix() + "\r\n");
    }
}