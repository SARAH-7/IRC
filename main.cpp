#include <cstdlib>
#include "Server.hpp"
#include "Client.hpp"
#include "Commands/Command.hpp"

int main(int ac, char **av) {
    if (ac != 3) {
        std::cout << "Invalid number of arguments!\n" << "Arguments needed: ./ircserv <port number> <password>" << std::endl;;
        return 1;
    }

    int port = std::atoi(av[1]);
    if (port < 6660 || port > 6669) {
        std::cout << "Invalid port range!" << std::endl;
        return 1;
    }

    Server server(port, av[2]);
    server.init();

    server.acceptClients();

    server.stop();

    return 0;
}
