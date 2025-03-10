#include "Channel.hpp"

Channel::Channel() : _name(""), 
                    _topic(""),
                    _password(""),
                    _limits(0),
                    _currentUsers(0)
{
    _modes['i'] = false;
    _modes['t'] = false;
    _modes['k'] = false;
    _modes['l'] = false;
}

void Channel::setTopic(const std::string &topic) { _topic = topic; }

void Channel::setName(const std::string &name) { _name = name; }

void Channel::setPassword(const std::string &pass) 
{
    _password = pass;
}

void Channel::setLimit(const int limit) { _limits = limit; }

void Channel::incrementCurrentUsers() { _currentUsers++; }

void Channel::setMode(const char mode, const bool value)
{
    if (_modes.find(mode) != _modes.end())
        _modes[mode] = value;
}

std::string Channel::getName() const { return (_name); }

std::string Channel::getPassword() const { return (_password); }

std::string Channel::getTopic() const { return (_topic); }

int Channel::getLimits() const { return (_limits); }

int Channel::getCurrentUsers() const { return (_currentUsers); }

bool Channel::getMode(char mode) const 
{
    std::map<char, bool>::const_iterator it = _modes.find(mode);
    return (it->second);
}

std::vector<Client *> Channel::getMembers() const
{
    std::vector<Client*> membersList;
    for (std::map<int, Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
        membersList.push_back(it->second);
    return (membersList);
}

std::vector<Client *> Channel::getOperators() const
{
    std::vector<Client *> operatorsList;
    for (std::map<int, Client*>::const_iterator it = _operators.begin(); it != _operators.end(); ++it)
        operatorsList.push_back(it->second);
    return (operatorsList);
}

void Channel::addMember(Client *client)
{
    if (_members.find(client->getFd()) == _members.end())
    {
        _members[client->getFd()] = client;
        _currentUsers++;
    }
}

void Channel::removeMember(Client *client)
{
    if (_members.find(client->getFd()) != _members.end())
    {
        _members.erase(client->getFd());
        _currentUsers--;
    }
    if (_operators.find(client->getFd()) != _operators.end())
        _operators.erase(client->getFd());
}

void Channel::giveOperatorPrivilage(Client *client)
{
    if(_members.find(client->getFd()) != _members.end())
        _operators[client->getFd()] = client;
}

void Channel::removeOperatorPrivilage(Client *client)
{
    if(_operators.find(client->getFd()) != _operators.end())
        _operators.erase(client->getFd());
}

bool Channel::isMember(int clientFd) const
{
    return _members.find(clientFd) != _members.end();
}

bool Channel::isOperator(int clientFd) const
{
    return _operators.find(clientFd) != _operators.end();
}

void Channel::addInvitedUser(Client *client) 
{
    _invitedUsers.push_back(client->getFd());
}

bool Channel::isInvited(Client *client)
{
    return std::find(_invitedUsers.begin(), _invitedUsers.end(), client->getFd()) != _invitedUsers.end();
}

void Channel::removeInvitedUser(Client *client)
{
    std::vector<int>::iterator it = std::find(_invitedUsers.begin(), _invitedUsers.end(), client->getFd());
    if (it != _invitedUsers.end())
        _invitedUsers.erase(it);
}

Channel::~Channel() {}