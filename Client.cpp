#include "Client.hpp"
Client::Client () {}

Client::Client(int fd) : socketfd(fd),
                        _isRegistered(false),
                        _isAuthenticated(false),
                        _isOperator(false),
                        _user(""),
                        _nickname(""),
                        _hostName(""),
                        _realName("") {}

                        
void Client::setIsRegistered(bool val) { this->_isRegistered = val; }

void Client::setIsAuthenticated(bool val) { this->_isAuthenticated = val; }

void Client::setIsOperator(bool val) { this->_isOperator = val; }

void Client::setUser(std::string &user) { this->_user = user; }

void Client::setNick(std::string &nickname) { this->_nickname = nickname; }

void Client::setHost(std::string &host) { this->_hostName = host; }

void Client::setRealName(std::string &realname) { this->_realName = realname; }

int Client::getFd() const { return (this->socketfd); }

bool Client::getIsRegistered() const { return (this->_isRegistered); }

bool Client::getIsAuthenticated() const { return (this->_isAuthenticated); }

bool Client::getIsOperator() const { return (this->_isOperator); }

std::string Client::getUser() const { return (this->_user); }

std::string Client::getNick() const { return (this->_nickname); }

std::string Client::getHost() const { return (this->_hostName); }

std::string Client::getRealName() const { return (this->_realName); }
                        
Client::~Client() {}

                    
