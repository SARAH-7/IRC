#include "Command.hpp"

void Command::execQuit()
{
	if(!client.getIsAuthenticated() || !client.getIsRegistered())
    {
        server.sendToClient(client.getFd(), RED "451: " + client.getNick() + " :You have not registered\r\n" RESET);
        return ;
    }

    std::string quitMessage;
    if (args.size() > 0)  
    {
        for(size_t i = 0; i < args.size(); i++)
        {
            quitMessage += args[i];
            if(args.size() - 1)
                quitMessage += ' ';
        }
    }
    else
        quitMessage = "Client Quit";
    std::string message = ":" + client.getNick() + " QUIT :" + client.getNick() + " has left the server: " + quitMessage + "\r\n";
    std::vector<std::string> channels = server.getClientChannels(client);
    for (size_t i = 0; i < channels.size(); i++)
    {
        Channel* channel = server.getChannel(channels[i]);
        if (channel)
            server.sendToChannel(client.getFd(), channel->getMembers(), message);
    }
    server.sendToClient(client.getFd(), RED "ERROR :Closing connection\r\n" RESET);
    server.disconnectClient(client.getFd());
}