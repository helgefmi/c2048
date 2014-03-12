CC=gcc
CPPFLAGS=-Wall -Wextra -Werror -std=gnu99 -O3

SRC=src/util.o src/board.o src/cache.o src/ai.o src/interface.o src/main.o src/test.o
TARGET=2048

all: $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(CPPFLAGS)

clean:
	rm -f $(TARGET) src/*.o
