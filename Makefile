COMPIL_SERVER = gcc
FLAG_SERVER = -Wall -pthread
NAME_SERVER = server

COMPIL_CLIENT = javac
FLAG_CLIENT =
NAME_CLIENT = Client

all: $(NAME_SERVER) $(NAME_CLIENT)

$(NAME_SERVER).o: $(NAME_SERVER).h $(NAME_SERVER).c
	$(COMPIL_SERVER) -c $(FLAG_SERVER) $(NAME_SERVER).c

$(NAME_SERVER): $(NAME_SERVER).o
	$(COMPIL_SERVER) $(FLAG_SERVER) $(NAME_SERVER).o -o $(NAME_SERVER)

$(NAME_CLIENT): $(NAME_CLIENT).java
	$(COMPIL_CLIENT) $(FLAG_CLIENT) $(NAME_CLIENT).java

clean:
	rm $(NAME_SERVER).o $(NAME_SERVER) $(NAME_CLIENT).class
