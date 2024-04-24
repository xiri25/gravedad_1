CC=gcc
CFLAGS=-Wall -Wextra

.PHONY: all clean

all: planetas

satisfactory: main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f planetas 
