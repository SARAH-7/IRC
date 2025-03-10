#include "Command.hpp"

void Command::applyMode(Channel *channel, char mode, bool enable, const std::string &modeType)
{
    switch (mode)
    {
        case 'i':
            channel->setMode('i', enable);
            server.sendToChannel(client.getFd(), channel->getMembers(), "Mode +i set on " + channel->getName() + "\n");
            break;
        case 't':
            channel->setMode('t', enable);
            server.sendToChannel(client.getFd(), channel->getMembers(), "Mode +t set on " + channel->getName() + "\n");
            break;
        case 'k':
            if (enable)
            {
                if (modeType.empty())
                {
                    server.sendToClient(client.getFd(), RED "461 MODE +k :Password required\n" RESET);
                    return;
                }
                channel->setPassword(modeType);
            }
            else
                channel->setPassword("");
            server.sendToChannel(client.getFd(), channel->getMembers(), "Mode +k set on " + channel->getName() + "\n");
            break;
        case 'o':{
            if (modeType.empty())
            {
                server.sendToClient(client.getFd(), RED "461 MODE +o :Nickname required\n" RESET);
                return;
            }
            Client* OpClient = server.getClientByNick(modeType);
            if (!OpClient || !channel->isMember(OpClient->getFd()))
            {
                server.sendToClient(client.getFd(), RED "441 " + modeType + " " + channel->getName() + " :They aren't in the channel\n" RESET);
                return;
            }
            if (enable)
                channel->giveOperatorPrivilage(OpClient);
            else
                channel->removeOperatorPrivilage(OpClient);
            server.sendToChannel(client.getFd(), channel->getMembers(), "Mode +o set on " + modeType + " in " + channel->getName() + "\n");
            break;
        }
        case 'l':
           if (enable)
           {
               if (modeType.empty() || !std::isdigit(modeType[0]))
               {
                   server.sendToClient(client.getFd(), RED "461 MODE +l :Invalid limit\n" RESET);
                   return;
               }
               int limit = std::atoi(modeType.c_str());
               channel->setLimit(limit);
           }
           else
               channel->setLimit(0);
           server.sendToChannel(client.getFd(), channel->getMembers(), "Mode +l set on " + channel->getName() + "\n");
           break;
        default:
           server.sendToClient(client.getFd(), RED "472 " + std::string(1, mode) + " :Unknown mode flag\n" RESET);
           break;
    }
}

void Command::execMode()
{
    if (args.empty())
    {
        server.sendToClient(client.getFd(), RED "461 MODE :Not enough parameters\n" RESET);
        return;
    }
    std::string channelName = args[0];
    Channel* channel = server.getChannel(channelName);
    if (!channel)
    {
        server.sendToClient(client.getFd(), RED "403 " + channelName + " :No such channel\n" RESET);
        return;
    }
    if (args.size() < 2)
    {
        server.sendToClient(client.getFd(), RED "461 MODE :Not enough parameters\n" RESET);
        return;
    }
    std::string modeFlag = args[1];
    std::string modeType = "";
    if(args.size() > 2)
        modeType = args[2];
    if (!channel->isOperator(client.getFd()))
    {
        server.sendToClient(client.getFd(), RED "482 " + channelName + " :You're not a channel operator\n" RESET);
        return;
    }
    bool enable = true;
    for (size_t i = 0; i < modeFlag.length(); i++)
    {
        char mode = modeFlag[i];
    
        if (mode == '+')
            enable = true;
        else if (mode == '-')
            enable = false;
        else
            applyMode(channel, mode, enable, modeType);
    }
}