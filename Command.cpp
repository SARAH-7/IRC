#include "Command.hpp"

Command::Command(std::string buffer, Client &client, Server &server) 
    : buffer(buffer), command(""), client(client), server(server) {}

Command::~Command() {}

void Command::parseBuffer() {
    if (buffer.empty()) return;

    std::stringstream ss(this->buffer);
    ss >> this->command;

    std::string arg;
    while (ss >> arg) {
        this->args.push_back(arg);
    }
}

void Command::executeCommand() {
    if (this->command == "USER") {
        execUser();
    } else if (this->command == "NICK") {
        execNick();
    }
}