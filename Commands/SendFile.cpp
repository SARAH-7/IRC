#include "Command.hpp"
#include <fstream>
#include <sstream>
#include "../base64.hpp"

void Command::execSendFile() {
    if (args.size() < 2) {
        server.sendToClient(client.getFd(), RED "Usage: SENDFILE <filename> <recipient>\n" RESET);
        return;
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
    // Read file contents
    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    // Encode file content to Base64
    std::string encoded = base64_encode(content);

    // Find recipient client
    Client* recipient = server.getClientByNick(recipientNick);
    if (!recipient) {
        server.sendToClient(client.getFd(), RED "Error: Recipient not found.\n" RESET);
        return;
    }

    // Send encoded file data to recipient
    std::string message = "FILE_TRANSFER: " + filename + " : " + encoded + "\n";
    server.sendToClient(recipient->getFd(), message);

    // Notify sender that the file was sent
    server.sendToClient(client.getFd(), GREEN "File sent successfully to " + recipientNick + "\n" RESET);
}
