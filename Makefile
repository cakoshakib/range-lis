OUTPUT=main
CFLAGS=-g -Wall
CFILES=main.cpp novel/*.cpp precompute/*.cpp

all: main

main: $(CFILES)
	g++ $(CFLAGS) -o main $(CFILES)

clean: 
	rm main
