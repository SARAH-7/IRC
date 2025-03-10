CPPFILES 	   = main.cpp  ./Commands/Command.cpp ./Commands/Nick.cpp ./Commands/User.cpp ./Commands/Invite.cpp ./Commands/Join.cpp ./Commands/Kick.cpp \
./Commands/Mode.cpp ./Commands/Part.cpp ./Commands/Pass.cpp ./Commands/Topic.cpp ./Commands/Help.cpp ./Commands/Quit.cpp Channel.cpp Client.cpp Server.cpp \

CPP	= c++

CPPFLAGS	= -Wall -Wextra -Werror -std=c++98

NAME	= IRC

all: $(NAME)

$(NAME): $(CPPFILES)
	$(CPP) $(CPPFLAGS) $(CPPFILES) -o $(NAME)

fclean:
	rm -f $(NAME)

re: fclean all
