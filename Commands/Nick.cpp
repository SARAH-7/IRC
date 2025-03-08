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
    // if(!client.getIsAuthenticated())
    // {
    //     std::cerr << "<client> Error 998: A password needs to be provided first to be authenticate" << std::endl;
    //     return ;
    // }
    if(args.empty())
    {
        server.sendToClient(client.getFd(),"431 :No nickname given\n");
        return ;
    }
    if(!isValidNick(args[0]))
    {
        server.sendToClient(client.getFd(), "432 " + args[0] + " :Erroneus nickname\n");
        return ; 
    }
    if(std::find(nicknameStored.begin(), nicknameStored.end(), args[0]) != nicknameStored.end())
    {
        server.sendToClient(client.getFd(),"433: " + args[0] + " :Nickname is already in use\n");
        return;
    }
    client.setNick(args[0]);
    nicknameStored.push_back(args[0]);
    if(!client.getUser().empty())
        client.setIsRegistered(true);
    std::string successMsg = "Your nickname had been set to " + client.getNick() + "\n";
    server.sendToClient(client.getFd(), successMsg);
}