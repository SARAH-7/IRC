#include "Command.hpp"

void Command::execUser()
{
    // if(!client.getIsAuthenticated())
    // {
    //     std::cerr << "Error 998: A password needs to be provided first to be authenticated" << std::endl;
    //     return ;
    // }
    if(client.getIsRegistered() || !client.getUser().empty())
    {
        server.sendToClient(client.getFd(), "462: You may not reregister\n");
        return ;
    }
    if(args.size() < 4 || args.empty())
    {
        server.sendToClient(client.getFd(), " Error: 461: Not enough parameters\n");
        return ;
    }
    client.setUser(args[0]);
    client.setHost(args[1]);
    client.setRealName(args[3]);
    if(!client.getNick().empty())
        client.setIsRegistered(true);
    std::string welcomeMsg = "Welcome to the IRC server " + client.getUser() + "\n";
    server.sendToClient(client.getFd(), welcomeMsg);
}