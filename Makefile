OUTPUT=main
CFLAGS=-g -Wall
CFILES=main.cpp novel/*.cpp precompute/*.cpp live/*.cpp

all: main

main: $(CFILES)
	g++ $(CFLAGS) -o main $(CFILES)

clean: 
	rm main
