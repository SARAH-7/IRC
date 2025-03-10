#include "Command.hpp"

void Command::execUser()
{
    if(!client.getIsAuthenticated())
    {
        server.sendToClient(client.getFd(),RED "451: :You have not registered\n" RESET);
        return ;
    }
    if(client.getIsRegistered() || !client.getUser().empty())
    {
        server.sendToClient(client.getFd(), RED "462: You may not reregister\n" RESET);
        return ;
    }
    if(args.size() < 4 || args.empty())
    {
        server.sendToClient(client.getFd(), RED "461: Not enough parameters\n" RESET);
        return ;
    }
    client.setUser(args[0]);
    client.setHost(args[1]);
    client.setRealName(args[3]);
    if(!client.getNick().empty())
    {
        client.setIsRegistered(true);
        std::string welcomeMsg = GREEN "Welcome to the IRC server " + client.getUser() + "\n" RESET;
        server.sendToClient(client.getFd(),  welcomeMsg);
    }
}