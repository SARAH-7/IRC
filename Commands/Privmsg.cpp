#include "Command.hpp"

void Command::execPrivMsg() {
    if (args.size() < 2) {
        // Not enough arguments (PRIVMSG <target> <message>)
        server.sendToClient(client.getFd(), "461 PRIVMSG :Not enough parameters\r\n");
        return;
    }
    
    std::string target = args[0]; // The target (user or channel)
    std::string message; // The message to send
    for(size_t i = 1; i < args.size(); i++)
    {
        message += args[i];
        if(args.size() - 1)
            message += ' ';
    }
    // Check if the target is a channel (starts with '#' or '&')
    if (target[0] == '#' || target[0] == '&') {
        // Send message to the channel
        Channel* channel = server.getChannel(target);
        if (channel) {
            // Check if the client is a member of the channel
            if (channel->isMember(client.getFd())) {
                // Broadcast the message to all members of the channel
                std::string formatted_message = ":" + client.getNick() + " PRIVMSG " + target + " :" + message + "\r\n";
                server.sendToChannel(channel->getMembers(), formatted_message);
            } else {
                server.sendToClient(client.getFd(), "404 " + target + " :Cannot send to channel (You're not a member)\r\n");
            }
        } else {
            server.sendToClient(client.getFd(), "403 " + target + " :No such channel\r\n");
        }
    } else {
        // Send private message to a user
        Client* recipient = server.getClientByNick(target);
        if (recipient) {
            std::string formatted_message = ":" + client.getNick() + " PRIVMSG " + target + " :" + message + "\r\n";
            server.sendToClient(recipient->getFd(), formatted_message);
        } else {
            server.sendToClient(client.getFd(), "401 " + target + " :No such nick\r\n");
        }
    }
    }