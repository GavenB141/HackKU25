PLATFORM ?= Windows
# Linux or Mac

COMP_FILES = src/main.c src/player.c src/level.c
CC = gcc

ifeq ($(PLATFORM), Windows)
	LDFLAGS := -lgdi32 -lwinmm
	COMP_FILES += src/libraylib.a
endif
ifeq ($(PLATFORM), Linux)
	LDFLAGS := -lraylib
endif
ifeq ($(PLATFORM), Mac)
	CC = eval cc
	LDFLAGS := $(shell pkg-config --libs --cflags raylib)
endif

BIN_NAME := game

$(BIN_NAME): $(COMP_FILES)
	$(CC) $(COMP_FILES) -o $(BIN_NAME) $(LDFLAGS) -Iinclude

clean:
	rm $(BIN_NAME)

run: $(BIN_NAME)
	./$(BIN_NAME)
