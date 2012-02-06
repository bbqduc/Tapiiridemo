###############################
# Update sources & headers here

SRC=gl3w.c
HEADERS=
MAIN=main.c
EXENAME=main

###############################

CC=gcc
CCFLAGS=-O3 -pedantic -Wall -Wextra -std=c99
INCLUDE=-Iglfw/include
LIBS=-Lglfw/lib/x11 -lglfw
OBJ=$(SRC:.c=.o)

all: $(MAIN) $(OBJ)
	$(CC) $(CCFLAGS) $(LIBS) $(MAIN) $(OBJ) -o $(EXENAME)
$(OBJ) : $(SRC) $(HEADERS)
%.o : %.c
	$(CC) $(CCFLAGS) -c -MD $< -o $@ $(INCLUDE) $(LIBS)
clean:
	-rm $(EXENAME)
	-rm *.d *.o
.PHONY: all clean
