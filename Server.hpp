#ifndef SERVER_H
# define SERVER_H
# include <iostream>

class Server {
	private:
	public:
		Server();
		Server(const std::string name, const std::string port, const std::string password);
		Server(const Server &copy);
		Server& operator=(const Server &other);
		~Server();

};

#endif