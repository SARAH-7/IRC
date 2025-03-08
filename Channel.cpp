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

std::string Channel::getTopic() const { return (_topic); }

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
    if(_modes['i'] && _operators.find(client->getFd()) == _operators.end())
    {
        std::cerr << "Error: Channel is invite-only" << std::endl;
        return;
    }
    if(_currentUsers > 0 && _currentUsers == _limits)
    {
        std::cerr << "Error: Maximum limit for channel reached" << std::endl;
        return ;
    }
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
    {
        _operators[client->getFd()] = client;
        std::cout << client->getFd() << " is not an operator" << std::endl;
    }
}

void Channel::removeOperatorPrivilage(Client *client)
{
    if(_operators.find(client->getFd()) != _operators.end())
    {
        _operators.erase(client->getFd());
        std::cout << client->getFd() << " is no longer an operator" << std::endl;
    }
}

Channel::~Channel() {}