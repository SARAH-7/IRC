#include "Server.hpp"
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <poll.h>

// Constructor
Server::Server(int p, const std::string& password) : _port(p), _password(password), _authenticated(false), _max_clients(10){
    _client_addr_len = sizeof(_client_addr);
}

// Initialize the server (create socket, bind, listen)
void Server::init() {
    _authenticated = true;
    _server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_fd < 0) {
        perror("socket");
        exit(1);
    }

    // Set socket options
    int opt = 1;
    setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Set up server address
    memset(&_server_addr, 0, sizeof(_server_addr));
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_addr.s_addr = INADDR_ANY;
    _server_addr.sin_port = htons(_port);

    // Bind the server socket
    if (bind(_server_fd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) < 0) {
        perror("bind");
        close(_server_fd);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(_server_fd, _max_clients) < 0) {
        perror("listen");
        close(_server_fd);
        exit(1);
    }

    std::cout << "Server listening on port " << _port << std::endl;
}

// Send message to a specific client
void Server::sendToClient(int client_fd, const std::string& message) {
    send(client_fd, message.c_str(), message.length(), 0);
}

// Send message to all clients in a channel (except sender)
void Server::sendToChannel(int sender_fd, const std::vector<Client*>& clients, const std::string& message) {
    std::vector<Client *>::const_iterator it;
    for (it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->getFd() != sender_fd) {
            sendToClient((*it)->getFd(), message);
        }
    }
}

// Authenticate client based on password
bool Server::authenticateClient(int client_fd, const std::string& received_password) {
    if (received_password == _password) {
        return true;
    } else {
        sendToClient(client_fd, "464 :Password incorrect\n");
        return false;
    }
}

Channel *Server::getChannel(const std::string &channelName)
{
    std::map<std::string, Channel *>::iterator it = _channels.find(channelName);
    if (it != _channels.end())
        return it->second;
    return NULL;
}

Channel *Server::createChannel(const std::string &channelName)
{
    Channel *newChannel = new Channel();
    newChannel->setName(channelName);
    _channels[channelName] = newChannel;
    return newChannel;
}

Client *Server::getClientByNick(const std::string &nickname)
{
    std::map<int, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second->getNick() == nickname)
            return it->second;
    }
    return NULL;
}

std::vector<std::string> Server::getClientChannels(Client &client)
{
    std::vector<std::string> clientChannels;
    std::map<std::string, Channel *>::iterator it;

    for (it = _channels.begin(); it != _channels.end(); ++it) 
    {
        if (it->second->isMember(client.getFd()))  
        {
            clientChannels.push_back(it->second->getName());  
        }
    }
    return clientChannels;
}

void Server::deleteChannel(const std::string &channelName)
{
    std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
    if (it != _channels.end())
    {
        Channel* channel = it->second;
        std::vector<Client*> members = channel->getMembers();
        for (size_t i = 0; i < members.size(); i++)
        {
            channel->removeMember(members[i]);
        }
        delete channel;
        _channels.erase(it);
        std::cout << "Channel " << channelName << " deleted from server." << std::endl;
    }
}

void Server::disconnectClient(int clientFd)
{
    std::map<int, Client *>::iterator it = _clients.find(clientFd);
    if (it != _clients.end())
    {
        close(clientFd);
        delete it->second;
        _clients.erase(it);
        std::cout << "Client " << clientFd << " disconnected from the server." << std::endl;
    }
}

// Accept and manage client connections using poll()
void Server::acceptClients() {
    std::vector<pollfd> fds;

    // Add server socket to pollfd list
    pollfd server_pollfd;
    server_pollfd.fd = _server_fd;
    server_pollfd.events = POLLIN;
    fds.push_back(server_pollfd);

    // Add stdin to detect Cmd+D (EOF)
    pollfd stdin_pollfd;
    stdin_pollfd.fd = STDIN_FILENO;
    stdin_pollfd.events = POLLIN;
    fds.push_back(stdin_pollfd);

    while (true) {
        int poll_count = poll(&fds[0], fds.size(), -1);
        if (poll_count < 0) {
            perror("poll");
            break;
        }

        // Check for new client connection
        if (fds[0].revents & POLLIN) {
            int client_fd = accept(_server_fd, (struct sockaddr *)&_client_addr, &_client_addr_len);
            if (client_fd < 0) {
                perror("accept");
                continue;
            }

            std::cout << "Client " << client_fd << " connected!" << std::endl;
            _clients[client_fd] = new Client(client_fd);
            _client_fds.push_back(client_fd);

            // Add new client to poll list
            pollfd client_pollfd;
            client_pollfd.fd = client_fd;
            client_pollfd.events = POLLIN;
            fds.push_back(client_pollfd);
        }

        // Check for Cmd+D (EOF) on stdin
        if (fds[1].revents & POLLIN) {
            char buffer[10];
            ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
            if (bytes_read == 0) {  // EOF detected (Cmd+D)
                std::cout << "Server shutting down (EOF received)..." << std::endl;
                return;
            }
        }

        // Handle clients
        for (size_t i = 2; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                char buffer[1024];
                ssize_t bytes_read = read(fds[i].fd, buffer, sizeof(buffer) - 1);
                if (bytes_read <= 0) {
                    // Client disconnected
                    std::cout << "Client disconnected" << std::endl;
                    close(fds[i].fd);
                    _clients[fds[i].fd];
                    _client_fds.erase(_client_fds.begin() + (i - 2));
                    fds.erase(fds.begin() + i);
                    --i;
                } else {
                    buffer[bytes_read] = '\0';
                    std::string message(buffer);
                    Client* client = _clients[fds[i].fd];
                    Command cmd(message, *client, *this);
                    cmd.parseBuffer();
                    cmd.executeCommand();
                }
            }
        }
    }

    stop();
}

// Stop the server and close connections
void Server::stop() {
    std::vector<int>::iterator it;
    for (it = _client_fds.begin(); it != _client_fds.end(); ++it) {
        close(*it);
    }
    for(std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
        delete it->second;
    for(std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
        delete it->second;
    _clients.clear();
    _channels.clear();
    close(_server_fd);
    std::cout << "Server stopped." << std::endl;
}