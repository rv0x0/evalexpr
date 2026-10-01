CC=cc
FLAGS=-Wall -Wextra -g -std=c11

all:
	$(CC) $(FLAGS) -o evalexpr ./evalexpr.c

clean:
	rm ./evalexpr

.PHONY: all
