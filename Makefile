CPPFILES 	   = main.cpp  \
			./Commands/Command.cpp ./Commands/Nick.cpp ./Commands/User.cpp ./Commands/Invite.cpp ./Commands/Join.cpp ./Commands/Kick.cpp \
			./Commands/Mode.cpp ./Commands/Part.cpp ./Commands/Pass.cpp ./Commands/Topic.cpp ./Commands/Help.cpp ./Commands/Quit.cpp \
			./Commands/Privmsg.cpp ./Commands/Notice.cpp \
			./Commands/SendFile.cpp ./Bot.cpp base64.cpp Channel.cpp Client.cpp Server.cpp  

CPP	= c++

CPPFLAGS	= -Wall -Wextra -Werror -std=c++98

NAME	= ircserv

OBJ = $(CPPFILES:.cpp=.o)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@ 

all: $(NAME)

$(NAME): $(OBJ)
	$(CPP) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) 

re: fclean all
