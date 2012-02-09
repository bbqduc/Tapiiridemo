###############################
# Update sources & headers here

SRC=shader.c model.cpp
HEADERS=shader.h model.h
MAIN=main.c
EXENAME=main

###############################

CC=g++
CCFLAGS=-ggdb -Wall -Wextra -std=c++0x
INCLUDE=-Iglfw/include
LIBS=-Lglfw/lib/x11 -lglfw -lGLU
OBJ=$(SRC:.c=.o)

all: $(MAIN) $(OBJ)
	$(CC) $(CCFLAGS) -Wl,-rpath,glfw/lib/x11,-rpath,. $(LIBS) $(MAIN) $(INCLUDE) $(OBJ) -o $(EXENAME) gl3w.o
$(OBJ) : $(SRC) $(HEADERS)
%.o : %.c
	$(CC) $(CCFLAGS) -c -MMD $< -o $@ $(INCLUDE) $(LIBS)
clean:
	-rm $(EXENAME)
	-rm *.d *.o
.PHONY: all clean
