#include "Server.hpp"
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <poll.h>

// Constructor
Server::Server(int p) : port(p), auth(false), max_clients(10) {
    client_addr_len = sizeof(client_addr);
}

// Initialize the server (create socket, bind, listen)
void Server::init() {
    auth = true;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(1);
    }

    // Set socket options
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind the server socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_fd, max_clients) < 0) {
        perror("listen");
        close(server_fd);
        exit(1);
    }

    std::cout << "Server listening on port " << port << std::endl;
}

// Send message to a specific client
void Server::sendToClient(int client_fd, const std::string& message) {
    send(client_fd, message.c_str(), message.length(), 0);
}

// Send message to all clients in a channel (except sender)
void Server::sendToChannel(int sender_fd, const std::vector<int>& clients, const std::string& message) {
    for (std::vector<int>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        if (*it != sender_fd) {
            sendToClient(*it, message);
        }
    }
}

// Accept and manage client connections using poll()
void Server::acceptClients() {
    std::vector<pollfd> fds;

    // Add server socket to pollfd list
    pollfd server_pollfd;
    server_pollfd.fd = server_fd;
    server_pollfd.events = POLLIN;
    server_pollfd.revents = 0;
    fds.push_back(server_pollfd);

    // Add stdin to detect Cmd+D (EOF)
    pollfd stdin_pollfd;
    stdin_pollfd.fd = STDIN_FILENO;
    stdin_pollfd.events = POLLIN;
    stdin_pollfd.revents = 0;
    fds.push_back(stdin_pollfd);

    while (true) {
        int poll_count = poll(&fds[0], fds.size(), -1);
        if (poll_count < 0) {
            perror("poll");
            break;
        }

        // Check for new client connection
        if (fds[0].revents & POLLIN) {
            int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
            if (client_fd < 0) {
                perror("accept");
                continue;
            }

            std::cout << "New client connected!" << std::endl;
            client_fds.push_back(client_fd);

            // Add new client to poll list
            pollfd client_pollfd;
            client_pollfd.fd = client_fd;
            client_pollfd.events = POLLIN;
            client_pollfd.revents = 0;
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
                    client_fds.erase(client_fds.begin() + (i - 2));
                    fds.erase(fds.begin() + i);
                    --i;
                } else {
                    buffer[bytes_read] = '\0';
                    std::cout << "Received message: " << buffer << std::endl;
                }
            }
        }
    }

    stop();
}

// Stop the server and close connections
void Server::stop() {
    for (std::vector<int>::iterator it = client_fds.begin(); it != client_fds.end(); ++it) {
        close(*it);
    }
    close(server_fd);
    std::cout << "Server stopped." << std::endl;
}
