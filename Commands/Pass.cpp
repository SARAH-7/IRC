#include "Command.hpp"

void Command::execPass()
{
    if(args.empty())
    {
        server.sendToClient(client.getFd(), RED "461 PASS :Not enough parameters\n" RESET);
        return ;
    }
    if(args.size() > 1)
    {
        server.sendToClient(client.getFd(), RED "461: " + client.getNick() + " :PASS : Wrong number of parameters : PASS <password>\r\n" RESET);
        return ;
    }
    if(client.getIsAuthenticated())
    {
        server.sendToClient(client.getFd(), RED "462 :You may not reregister\n" RESET);
        return ;
    }
    if (server.authenticateClient(client.getFd(), args[0]))
    {
        client.setIsAuthenticated(true);
        return ;
    }
}