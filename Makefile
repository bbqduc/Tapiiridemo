###############################
# Update sources & headers here

SRC=shader.cpp model.cpp snd.cpp particle.cpp glutils.cpp concurrency.cpp oclfile.cpp
HEADERS=shader.h model.h snd.h particle.h glutils.h concurrency.h oclfile.h
MAIN=main.cpp
EXENAME=main

###############################

CC=g++
CCFLAGS=-Wall -Wextra -std=c++0x -DGLM_FORCE_CXX11 -Os
INCLUDE=-I./glfw/include -I/usr/local/cuda/include/
LIBS=-Lglfw/lib/x11 -lglfw -lGLU -lbass -lOpenCL -L/usr/local/cuda/lib/ -L.
OBJ=$(SRC:.cpp=.o)

all: $(MAIN) $(OBJ) gl3w.o
	$(CC) $(CCFLAGS) -Wl,-rpath,glfw/lib/x11,-rpath,. $(LIBS) $(MAIN) $(INCLUDE) $(OBJ) -o $(EXENAME) gl3w.o
$(OBJ) : $(SRC) $(HEADERS)
%.o : %.cpp
	$(CC) $(CCFLAGS) -c -MMD $< -o $@ $(INCLUDE) $(LIBS)
gl3w.o : gl3w.c
	$(CC) -c $< $(INCLUDE) -fpermissive
clean:
	-rm $(EXENAME)
	-rm *.d *.o
.PHONY: all clean
