#include "Command.hpp"

void Command::execQuit()
{
    std::string quitMessage = "Client Quit";
    if (args.size() > 0)  
        quitMessage = args[0];
    std::string message = ":" + client.getNick() + " QUIT :" + quitMessage + "\n";

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