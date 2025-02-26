#include "Command.hpp"

void Command::execUser()
{
    // if(!client.getIsAuthenticated())
    // {
    //     std::cerr << "Error 998: A password needs to be provided first to be authenticated" << std::endl;
    //     return ;
    // }
    if(client.getIsRegistered() || !client.getUser().empty())
    {
        std::cerr << "<client> Error 462: :You may not reregister" << std::endl;
        return ;
    }
    if(args.size() < 4 || args.empty())
    {
        std::cerr << "<client> " << command << " Error: 461: Not enough parameters" << std::endl;
        return ;
    }
    client.setUser(args[0]);
    client.setHost(args[1]);
    client.setRealName(args[3]);
    if(!client.getNick().empty())
        client.setIsRegistered(true);
    std::cout << client.getUser() << " :Welcome to the IRC server" << std::endl;
    std::cout << "Username: " << client.getUser() << std::endl;
    std::cout << "Hostname: " << client.getHost() << std::endl;
    std::cout << "Realname: " << client.getRealName() << std::endl;
}