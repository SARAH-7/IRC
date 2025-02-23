#include <iostream>
#include <cstdlib>
#include <cctype>
#include "Server.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

int main(int ac, char **av)
{
    (void)av;
    if (ac != 3)
        return (std::cout<<"Invalid Num of Args"<<std::endl, 1);
    while (av[--ac] && ac != 0)
    {
        int i = -1;
        while (av[ac][++i])
        {
            if (!std::isdigit(av[ac][i]))
                return (std::cout<<"Invalid Entry"<<std::endl, 1);
        }
    }
    // Server* server_instance = new Server("OURS", port, password);
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[1024];
    int port = std::atoi(av[1]);
    if (port < 6660 || port > 6669)
        return (0);
	int i = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(int));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    std::cout << "Server listening on port " << port << std::endl;

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return 1;
    }

    std::cout << "Client connected" << std::endl;

    while (true)
    {
        ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
        if (bytes_read == 0)
            break ;
        if (bytes_read < 0) {
            perror("read");
            close(client_fd);
            close(server_fd);
            return 1;
        }
        buffer[bytes_read] = '\0';
        std::cout << "Received message: " << buffer << std::endl;

    }
}