#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include "Client.hpp"

class Channel{
    private:
    std::string _name;
    std::string _topic;
    std::string _password;
    int     _limits;
    int     _currentUsers;
    std::map<char, bool> _modes;
    std::map<int, Client *> _members;
    std::map<int, Client *> _operators;
    std::vector<int> _invitedUsers;

    public:
    Channel();
    ~Channel();
    void setTopic(const std::string &topic);
    void setName(const std::string &name);
    void setPassword(const std::string &pass);
    void setMode(char mode, bool value);
    void setLimit(const int limit);
    void incrementCurrentUsers();

    std::string getName() const;
    std::string getTopic() const;
    std::string getPassword() const;
    int getCurrentUsers() const;
    int getLimits() const;
    bool getMode(char mode) const;
    bool isMember(int clientFd) const;
    bool isOperator(int clientFd) const;
    std::vector<Client *> getMembers() const;
    std::vector<Client *> getOperators() const;

    void addMember(Client *client);
    void removeMember(Client *client);
    void giveOperatorPrivilage(Client *client);
    void removeOperatorPrivilage(Client *client);

    void addInvitedUser(Client *client);
    bool isInvited(Client *client);
    void removeInvitedUser(Client *client);
};

#endif