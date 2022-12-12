CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -fsanitize=address -g
VPATH = src

TARGET = main
OBJS = main.o file_s.o

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

debug: CFLAGS += -g -fsanitize=address
debug: clean $(TARGET)

clean:
	$(RM) $(TARGET) $(OBJS)

.PHONY: all $(TARGET_LIB) clean
