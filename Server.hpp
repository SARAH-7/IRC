#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <vector>

class Server {
private:
    int port;
    int server_fd;
    bool auth;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;
    int max_clients;
    std::vector<int> client_fds;

public:
    Server(int p);                     
    void init();         
    void acceptClients();
	void sendToClient(int client_fd, const std::string &message);
	void sendToChannel(int sender_fd, const std::vector<int>& clients, const std::string& message);
    void stop();         
};

#endif
