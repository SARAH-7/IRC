#include <iostream>
#include <cstdlib>
#include "Server.hpp"

int main(int ac, char **av) {
    if (ac != 3) {
        std::cout << "Invalid number of arguments!" << std::endl;
        return 1;
    }

    int port = std::atoi(av[1]);
    if (port < 6660 || port > 6669) {
        std::cout << "Invalid port range!" << std::endl;
        return 1;
    }

    // Create server object
    Server server(port);

    // Initialize the server (create socket, bind, listen)
    server.init();

    // Accept and handle client connections
    server.acceptClients();

    // Stop the server when done
    server.stop();

    return 0;
}
