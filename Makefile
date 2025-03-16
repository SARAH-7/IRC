CPPFILES 	   = main.cpp  \
			./Commands/Command.cpp ./Commands/Nick.cpp ./Commands/User.cpp ./Commands/Invite.cpp ./Commands/Join.cpp ./Commands/Kick.cpp \
			./Commands/Mode.cpp ./Commands/Part.cpp ./Commands/Pass.cpp ./Commands/Topic.cpp ./Commands/Help.cpp ./Commands/Quit.cpp \
			./Commands/Privmsg.cpp ./Commands/Notice.cpp \
			./Commands/SendFile.cpp ./Bot.cpp base64.cpp Channel.cpp Client.cpp Server.cpp  

CPP	= c++

CPPFLAGS	= -Wall -Wextra -Werror -std=c++98 -g3

NAME	= ircserv

all: $(NAME)

$(NAME): $(CPPFILES)
	$(CPP) $(CPPFLAGS) $(CPPFILES) -o $(NAME)

bonus: $(NAME)
	$(CPP) $(CPPFLAGS) ./Bot.cpp ./Commands/Command.cpp -o $(NAME)  # ✅ Bonus: Compile Bot & File Transfer

fclean:
	rm -f $(NAME)

re: fclean all
