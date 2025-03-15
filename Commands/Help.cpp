#include "Command.hpp"


void Command::execHelp()
{
    std::string response = CYAN "╔════════════════════════════════════════════════════╗\n" RESET;
    response += CYAN "║            📜  Available Commands 📜               ║\n" RESET;
    response += CYAN "╠════════════════════════════════════════════════════╣\n" RESET;
    response += GREEN "║ 1️⃣   JOIN <#channel>          │ Join a channel      ║\n" RESET;
    response += GREEN "║ 2️⃣   PART <#channel>          │ Leave a channel     ║\n" RESET;
    response += GREEN "║ 3️⃣   QUIT                     │ Disconnect from IRC ║\n" RESET;
    response += GREEN "║ 4️⃣   INVITE <user> <#channel> │ Invite a user       ║\n" RESET;
    response += GREEN "║ 5️⃣   TOPIC <#channel> [topic] │ Set/view topic      ║\n" RESET;
    response += GREEN "║ 6️⃣   KICK <#channel> <user>   │ Remove a user       ║\n" RESET;
    response += GREEN "║ 7️⃣   MODE <#channel> <mode>   │ Change settings     ║\n" RESET;
    response += CYAN "╚════════════════════════════════════════════════════╝\n" RESET;

    server.sendToClient(client.getFd(), response);
}

