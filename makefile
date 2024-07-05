CC=gcc
CFLAGS=-Wall -Wextra
LDFLAGS=-lm -g
SRC_FILES=main.c\
		  math_utils/math_utils.c \
		  points/points.c \
		  result/write.c \
		  gravity/gravity.c \
		  setup/setup.c \
		  test/test.c \
		  test/two_b_problem.c \
		  test/test_desmos.c \
		  test/three_body.c

.PHONY: all clean

all: planetas

planetas: $(SRC_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f planetas 
