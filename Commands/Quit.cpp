#include "Command.hpp"

void Command::execQuit()
{
    std::string quitMessage = "Client Quit";
    if (args.size() > 0)  
    {
        for (size_t i = 1; i < args.size(); i++)
        {
            if (i > 1)
                quitMessage += " ";
            quitMessage += args[i];
        }
    }
    std::string message = ":" + client.getNick() + " QUIT :" + quitMessage + "\n";

    std::vector<std::string> channels = server.getClientChannels(client);
    for (size_t i = 0; i < channels.size(); i++)
    {
        Channel* channel = server.getChannel(channels[i]);
        if (channel)
        {
            server.sendToChannel(channel->getMembers(), message);
            channel->removeMember(&client);
            if (channel->getMembers().empty())  
                server.deleteChannel(channel->getName());
        }
    }
    server.sendToClient(client.getFd(), RED "ERROR :Closing connection\n" RESET);
    server.disconnectClient(client.getFd());
}