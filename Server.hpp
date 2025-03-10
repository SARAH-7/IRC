#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include "Client.hpp"
#include "./Commands/Command.hpp"
#include "Channel.hpp"

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

class Server {
private:
    int _port;
    std::string _password;
    int _server_fd;
    bool _authenticated;
    struct sockaddr_in _server_addr;
    struct sockaddr_in _client_addr;
    socklen_t _client_addr_len;
    int _max_clients;
    std::vector<int> _client_fds;
    std::map<int, Client *> _clients;
    std::map<int, Channel *> _channels;

public:
    Server(int port, const std::string& password);                     
    void init();         
    void acceptClients();
    bool authenticateClient(int client_fd, const std::string& received_password);
    void sendToClient(int client_fd, const std::string &message);
    void sendToChannel(int sender_fd, const std::vector<Client*>& clients, const std::string& message);
    void stop();
      
    Channel *getChannel(const std::string &channelName);
    Channel *createChannel(const std::string &channelName);
    Client* getClientByNick(const std::string& nickname);
    std::vector<std::string> getClientChannels(Client &client);

    void deleteChannel(const std::string& channelName);
    void disconnectClient(int clientFd);
};

#endif
