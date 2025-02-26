#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Channel;

class Client{
    private:
    int socketfd;
    bool _isRegistered;
    bool _isAuthenticated;
    bool _isOperator;
    std::string _user;
    std::string _nickname;
    std::string _hostName;
    std::string _realName;

    public:
    Client();
    Client(int fd);
    ~Client();

    //Setter Functions
    void setIsRegistered(bool val);
    void setIsAuthenticated(bool val);
    void setIsOperator(bool val);
    void setUser(std::string &user);
    void setNick(std::string &nickname);
    void setHost(std::string &host);
    void setRealName(std::string &realname);

    //Getter Functions
    int getFd() const;
    bool getIsRegistered() const;
    bool getIsAuthenticated() const;
    bool getIsOperator() const;
    std::string getUser() const;
    std::string getNick() const;
    std::string getHost() const;
    std::string getRealName() const;
};

#endif