CC=g++
CFLAGS=-Wall -lsfml-graphics -lsfml-window -lsfml-system 

all: 
	$(CC) -o pong main.cpp $(CFLAGS)
