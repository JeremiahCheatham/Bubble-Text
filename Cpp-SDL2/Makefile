CC      = g++
CFLAGS  = `pkg-config --cflags sdl2 SDL2_ttf` -std=c++20 -ggdb3 -Wall -Wextra -Wwrite-strings
LDLIBS  = `pkg-config --libs sdl2 SDL2_ttf` -lfmt
TARGET  = bubble-text

SRCS    = $(wildcard *.cpp)
OBJS    = $(SRCS:.cpp=.o)

.PHONY: all
all: $(TARGET)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(LDLIBS) $^ -o $@

.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGET)
