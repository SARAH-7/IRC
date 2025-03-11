#include "Command.hpp"

Command::Command(std::string buffer, Client &client, Server &server) : buffer(buffer), command(""), client(client), server(server) {}

Command::~Command() {}

//fix parsing for newline
void Command::parseBuffer()
{
    if(buffer.empty())
        return ;
    
    std::stringstream ss(this->buffer);
    ss >> this->command;

    std::string arg;
    while(ss >> arg)
        this->args.push_back(arg);
}


void Command::executeCommand()
{
    if (!client.getIsRegistered())
    {
        if (this->command == "PASS")
            execPass();
        else if (this->command == "USER")
            execUser();
        else if (this->command == "NICK")
            execNick();
        else if (command == "QUIT")
            execQuit();
        else
            server.sendToClient(client.getFd(), RED "You must register with PASS, USER, and NICK before using other commands.\n If you give up and want to leave use the QUIT command\n" RESET);
    }
    else
    {
        if (this->command == "JOIN")
            execJoin();
        else if (this->command == "PART")
            execPart();
        else if (this->command == "TOPIC")
            execTopic();
        else if (this->command == "KICK")
            execKick();
        else if (this->command == "INVITE")
            execInvite();
        else if (this->command == "MODE")
            execMode();
        else if (command == "QUIT")
            execQuit();
        else if(command == "HELP")
            execHelp();
		else if (this->command == "PRIVMSG")
			execPrivMsg();
        else
            server.sendToClient(client.getFd(), RED "Unknown command: " + this->command + " If you need assisatnce use the HELP command\n" RESET);
    }
}

void Command::execPrivMsg() {
if (args.size() < 2) {
	// Not enough arguments (PRIVMSG <target> <message>)
	server.sendToClient(client.getFd(), "461 PRIVMSG :Not enough parameters\r\n");
	return;
}

std::string target = args[0]; // The target (user or channel)
std::string message = args[1]; // The message to send

// Check if the target is a channel (starts with '#' or '&')
if (target[0] == '#' || target[0] == '&') {
	// Send message to the channel
	Channel* channel = server.getChannel(target);
	if (channel) {
		// Check if the client is a member of the channel
		if (channel->isMember(client.getFd())) {
			// Broadcast the message to all members of the channel
			std::string formatted_message = ":" + client.getNick() + " PRIVMSG " + target + " :" + message + "\r\n";
			server.sendToChannel(client.getFd(), channel->getMembers(), formatted_message);
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



