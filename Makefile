CC = gcc
CFLAGS = -Wall -O2 -std=c99 -Iinclude
TARGET = main
SRC = main.c src/neural.c src/matrix.c src/img.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
