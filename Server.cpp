#include "Server.hpp"
#include "Bot.hpp"
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <poll.h>

// Disable Ctrl+D as EOF
void Server::disableEOFBehavior() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term); // Get current terminal settings
    _original_termios = term;       // Save original settings

    // Disable Ctrl+D as EOF
    term.c_cc[VEOF] = _POSIX_VDISABLE;

    // Set terminal to raw mode
    term.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term); // Apply new settings
}

// Restore original terminal settings
void Server::restoreEOFBehavior() {
    tcsetattr(STDIN_FILENO, TCSANOW, &_original_termios);
}

// Constructor
Server::Server(int p, const std::string& password) 
    : _port(p), _password(password), _authenticated(false), _client_addr_len(sizeof(_client_addr)), _max_clients(10){
    std::signal(SIGTSTP, SIG_IGN);
    disableEOFBehavior();
	bot = new Bot(*this); 
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
// Destructor
Server::~Server() {
    restoreEOFBehavior(); // Restore original terminal settings
	delete bot;
}

// Send message to a specific client
void Server::sendToClient(int client_fd, const std::string& message)
{
    send(client_fd, message.c_str(), message.length(), 0);
}

// Authenticate client based on password
bool Server::authenticateClient(int client_fd, const std::string& received_password)
{
    if (received_password == _password) {
        return true;
    } else {
        sendToClient(client_fd, "464 :Password incorrect\n");
        return false;
    }
}

void Server::sendWelcomeMessage(int clientFd)
{
    std::string message = CYAN "╔═════════════════════════════════════════════════════════╗\n" RESET;
    message += CYAN "║              Welcome to ChatOasis Server! 🎉            ║\n" RESET;
    message += CYAN "╠═════════════════════════════════════════════════════════╣\n" RESET;
    message += GREEN "║   To start, you need to register:                       ║\n" RESET;
    message += GREEN "║  1️⃣  PASS <password>  - Authenticate yourself            ║\n" RESET;
    message += GREEN "║  2️⃣  NICK <nickname>  - Choose a nickname                ║\n" RESET;
    message += GREEN "║  3️⃣  USER <username> <hostname> <server> <realname>      ║\n" RESET;
    message += GREEN "║   Once registered, type /HELP for commands! 🚀          ║\n" RESET;
    message += CYAN "╚═════════════════════════════════════════════════════════╝\n" RESET;

    sendToClient(clientFd, message);
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
        Client* client = it->second;
        std::vector<std::string> clientChannels = getClientChannels(*client);
        for (size_t i = 0; i < clientChannels.size(); i++)
        {
            Channel* channel = getChannel(clientChannels[i]);
            if (channel)
            {
                channel->removeMember(client);
                if (channel->getMembers().empty())  
                    deleteChannel(channel->getName());
            }
        }
        if (!client->getNick().empty())
            removeNick(client->getNick());
        close(clientFd);
        delete client;
        _clients.erase(it);
        std::cout << "Client " << clientFd << " disconnected from the server." << std::endl;
    }
}

bool Server::isNickInUse(const std::string& nick)
{
    return _nicknames.find(nick) != _nicknames.end();
}

void Server::addNick(const std::string& nick)
{
    _nicknames.insert(nick);
}

void Server::removeNick(const std::string &nick)
{
    _nicknames.erase(nick);
}

// Accept and manage client connections using poll()
// Initialize the static flag
volatile sig_atomic_t Server::sigint_received = 0;

// Signal handler for SIGINT
void Server::handle_sigint(int signal) {
    if (signal == SIGINT) {
        sigint_received = 1;
    }
}

void Server::acceptClients() {
    std::vector<pollfd> fds;
    static std::map<int, std::string> commandBuffers;

    // Add server socket to pollfd list
    pollfd server_pollfd;
    server_pollfd.fd = _server_fd;
    server_pollfd.events = POLLIN;
    fds.push_back(server_pollfd);

    // Add stdin to detect input (e.g., Ctrl+D or commands)
    pollfd stdin_pollfd;
    stdin_pollfd.fd = STDIN_FILENO;
    stdin_pollfd.events = POLLIN;
    fds.push_back(stdin_pollfd);

    while (true) {
        // Check if SIGINT (^C) was received
        if (sigint_received) {
            std::cout << "Server shutting down (^C received)..." << std::endl;
            stop();
            return;
        }

        int poll_count = poll(&fds[0], fds.size(), -1);
        if (poll_count < 0) {
            if (errno == EINTR) {
                // poll was interrupted by a signal, check sigint_received
                continue;
            }
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
            if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) 
            {
                close(client_fd);
                throw std::runtime_error(std::string("fcntl() error: ") + strerror(errno));
            }
            std::cout << "Client " << client_fd << " connected!" << std::endl;
			// Send welcome message
			sendWelcomeMessage(client_fd);

			// Store client information
			_clients[client_fd] = new Client(client_fd);
			_client_fds.push_back(client_fd);

			// Add new client to poll list
			pollfd client_pollfd;
			client_pollfd.fd = client_fd;
			client_pollfd.events = POLLIN | POLLOUT;  // Allow both reading and writing
            client_pollfd.revents = 0;
			fds.push_back(client_pollfd);

        }

        // Check for input from stdin (e.g., commands or Ctrl+D)
        if (fds[1].revents & POLLIN) {
            char buffer[1024];
            ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
            if (bytes_read < 0)
                perror("read");
        }

        // Handle clients
        for (size_t i = 2; i < fds.size(); ++i) {
            if (fds[i].fd == -1)  // Skip invalid entries
                continue;
            if (fds[i].revents & POLLIN) {
                int client_fd = fds[i].fd;
                char buffer[1024];
                ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
                if (bytes_read < 0) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                        continue; 
                    else
                    {
                        std::cout << "recv() error: " << strerror(errno) << std::endl;
                        // Client disconnected
                        disconnectClient(client_fd);
                        commandBuffers.erase(client_fd);
                        fds.erase(fds.begin() + i);
                        close(client_fd);
                        --i;
                    }
                } else if (bytes_read == 0) {
                    std::cout << "Socket " << client_fd << " hung up" << std::endl;
                    disconnectClient(client_fd);
                    commandBuffers.erase(client_fd);
                    fds.erase(fds.begin() + i);
                    close(client_fd);
                    --i;

                } else {
                    buffer[bytes_read] = '\0';
                    commandBuffers[client_fd] += buffer;
                    size_t pos;
                     while ((pos = commandBuffers[client_fd].find('\n')) != std::string::npos) {
                        std::string message = commandBuffers[client_fd].substr(0, pos);
                        commandBuffers[client_fd].erase(0, pos + 1);  // Remove processed message from buffer

                        // Remove \r if present (handling \r\n cases)
                        if (!message.empty() && message[message.size() - 1] == '\r') {
                            message.erase(message.size() - 1);
                        }
                        Client* client = _clients[client_fd];
                        Command cmd(message, *client, *this, *bot);
                        cmd.parseBuffer();
                        cmd.executeCommand();
                    }
                }
            }
        }
    }
    stop();
}

// Stop the server and close connections
void Server::stop() {
    // Close all client sockets
    for (std::vector<int>::iterator it = _client_fds.begin(); it != _client_fds.end(); ++it) {
        close(*it);
    }

    // Free all Client objects
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        delete it->second;
    }

    // Free all Channel objects
    for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        delete it->second;
    }

    // Clear containers
    _clients.clear();
    _channels.clear();
    _client_fds.clear();

    // Close the server socket
    close(_server_fd);
}
void Server::sendToChannel(int sender_fd, const std::vector<Client*>& clients, const std::string& message) {
    std::vector<Client *>::const_iterator it;
    for (it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->getFd() != sender_fd) {
            sendToClient((*it)->getFd(), message);
        }
    }
}