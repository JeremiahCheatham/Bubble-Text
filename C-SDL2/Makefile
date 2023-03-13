CC      = gcc
CFLAGS  != pkg-config --cflags sdl2 SDL2_ttf
CFLAGS  += -std=c99 -ggdb3 -Wall -Wextra -Wwrite-strings
LDLIBS  != pkg-config --libs sdl2 SDL2_ttf
TARGET  = bubble-text

SRCS    = $(wildcard *.c)
OBJS    = $(SRCS:.c=.o)

.PHONY: all
all: $(TARGET)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(LDLIBS) $^ -o $@

.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGET)
