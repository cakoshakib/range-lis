OUTPUT=main
CFLAGS=-g -Wall

all: main

main: main.cpp novel/rlis.cpp
	g++ $(CFLAGS) -o main main.cpp novel/rlis.cpp
