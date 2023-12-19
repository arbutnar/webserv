NAME = webserv
SRC = Master/*.cpp Server/*.cpp Location/*.cpp Directives/*.cpp Request/*.cpp Response/*.cpp main.cpp
OBJ = obj/*.o
FLG = -Wextra -Wall -Werror -std=c++98

$(NAME): $(SRC)
	mkdir -p obj
	-c++ $(SRC) $(FLG) -c
	mv *.o obj
	c++ $(OBJ) -o $(NAME)

all:	$(NAME)

clean:
	rm -rf obj

fclean:	clean
	rm -rf $(NAME)

re:	fclean all