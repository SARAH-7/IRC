#include "Command.hpp"
#include <fstream>
#include <sstream>
#include "../base64.hpp"

void Command::execSendFile() {

	if(!client.getIsAuthenticated() || !client.getIsRegistered())
    {
        server.sendToClient(client.getFd(), RED "451: " + client.getNick() + " :You have not registered\r\n" RESET);
        return ;
    }

    if (args.size() < 2) {
        server.sendToClient(client.getFd(), RED "Usage: SENDFILE <filename> <recipient>\n" RESET);
        return;
    }

    if(args.size() > 2)
    {
        server.sendToClient(client.getFd(), RED "461: " + client.getNick() + " :SENDFILE : Wrong number of parameters : SENDFILE  <filename> <recipient>\r\n" RESET);
        return ;
    }
    std::string filename = args[0];
    std::string recipientNick = args[1];

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        std::cout << "Server is running in directory: " << cwd << std::endl;
    }

    std::ifstream file(filename.c_str(), std::ios::binary);
    if (!file.is_open()) {
        server.sendToClient(client.getFd(), "Error: Could not open file: " + filename + "\n");
        std::cout << "🔥 DEBUG: File '" << filename << "' not found in " << cwd << std::endl;
        return;
	}
    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    std::string encoded = base64_encode(content);

    Client* recipient = server.getClientByNick(recipientNick);
    if (!recipient) {
        server.sendToClient(client.getFd(), RED "Error: Recipient not found.\n" RESET);
        return;
    }

    std::string message = "FILE_TRANSFER: " + filename + " : " + encoded + "\n";
    server.sendToClient(recipient->getFd(), message);

    server.sendToClient(client.getFd(), GREEN "File sent successfully to " + recipientNick + "\n" RESET);
}
