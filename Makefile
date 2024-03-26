CC=gcc -g
CFLAGS=-I. -Wall
LIBS=-lglfw -lGL -lm -pthread
SRC=$(wildcard ./*.c)
OBJ=$(SRC:.c=.o)
TARGET=main

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: clean
