WARNS=-Wall -Wextra -g
LIBS=-lraylib -lm
CC=gcc
all:
	mkdir -p ./build/
	$(CC) -o build/shadow *.c $(WARNS) $(LIBS)
clean:
	rm -rf ./build
