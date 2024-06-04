CC=gcc
CFLAGS=-Wall -Wextra
LDFLAGS=-lm -g

.PHONY: all clean

all: planetas

planetas: main.c math_utils/math_utils.c points/points.c result/write.c gravity/gravity.c setup/setup.c test/test.c test/two_b_problem.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f planetas 
