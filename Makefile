CC := clang
CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -lSDL2_image -lm

# Header files
HDS := $(wildcard *.h)

# Source files
SRC := $(wildcard *.c)

# Object filenames
OBJ := $(SRC:.c=.o)

# Executable name
EXEC := game

all: $(EXEC)

$(EXEC): $(OBJ) $(HDS) Makefile
	$(CC) -o $@ $(OBJ) $(CFLAGS)

clean:
	rm -f $(EXEC) $(OBJ)

.PHONY: all clean
