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
        server.sendToClient(client.getFd(), RED "451: :You have not registered\n" RESET);
        return ;
    }
    if(args.empty())
    {
        server.sendToClient(client.getFd(), RED "431 :No nickname given\n" RESET);
        return ;
    }
    if(!isValidNick(args[0]))
    {
        server.sendToClient(client.getFd(), RED "432 " + args[0] + " :Erroneus nickname\n" RESET);
        return ; 
    }
    if(std::find(nicknameStored.begin(), nicknameStored.end(), args[0]) != nicknameStored.end())
    {
        server.sendToClient(client.getFd(), RED "433: " + args[0] + " :Nickname is already in use\n" RESET);
        return;
    }
    client.setNick(args[0]);
    nicknameStored.push_back(args[0]);
    std::string successMsg = "Your nickname had been set to " + client.getNick() + "\n";
    server.sendToClient(client.getFd(), successMsg);
    if(!client.getUser().empty())
    {
        client.setIsRegistered(true);
        std::string welcomeMsg = GREEN "Welcome to the IRC server " + client.getUser() + "\n" RESET;
        server.sendToClient(client.getFd(), welcomeMsg);
    }
}