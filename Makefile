CPPFILES 	   = main.cpp  ./Commands/Command.cpp ./Commands/Nick.cpp ./Commands/User.cpp Client.cpp Server.cpp \

CPP	= c++

CPPFLAGS	= -Wall -Wextra -Werror -std=c++98

NAME	= ircserv

all: $(NAME)

$(NAME): $(CPPFILES)
	$(CPP) $(CPPFLAGS) $(CPPFILES) -o $(NAME)

fclean:
	rm -f $(NAME)

re: fclean all
