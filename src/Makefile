CC = gcc
FLAGS = -Wall -Werror -Wextra
BUILD = ../build/

all: game1 game2

game1: game_of_life.c
	$(CC) $(FLAGS) game_of_life.c -lncurses -o $(BUILD)game_of_life

game2: game_of_life2.c
	$(CC) $(FLAGS) game_of_life2.c -lncurses -o $(BUILD)game_of_life_menu

clean:
	rm -rf *.o $(BUILD)*

rebuild: clean all