CC=gcc
CFLAGS=-Wall -Wextra
LDFLAGS=-lm

.PHONY: all clean

all: planetas

planetas: main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f planetas 
