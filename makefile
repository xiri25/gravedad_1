CC=gcc
CFLAGS=-Wall -Wextra
LDFLAGS=-lm -g

.PHONY: all clean

all: planetas

planetas: main.c points/points.c result/write.c gravity/gravity.c setup/setup.c test/test.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f planetas 
