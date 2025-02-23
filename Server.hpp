#ifndef SERVER_H
# define SERVER_H
# include <iostream>

class Server {
	private:
		Server();
		Server(const Server &copy);
		Server& operator=(const Server &other);
		~Server();
	public:
		Server(const std::string name, const std::string port, const std::string password);

};

#endif