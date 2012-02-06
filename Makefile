CC=gcc
CCFLAGS=-O3 -pedantic -Wall -Wextra -std=c99 -I glfw/include/ -L glfw/lib/x11/
LIBS=-lglfw
all: main.o gl3w.o
	$(CC) $(CCFLAGS) $(LIBS) main.o gl3w.o -o main

main.o:
	$(CC) $(CCFLAGS) -c main.c
gl3w.o:
	$(CC) $(CCFLAGS) -c gl3w.c

.PHONY: clean
clean:
	-rm *.o
