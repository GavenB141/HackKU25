PLATFORM ?= Windows
# Linux or Mac

COMP_FILES = src/main.c
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
	LDFLAGS := $(pkg-config --libs --cflags raylib)
endif

BIN_NAME := game

$(BIN_NAME): src/main.c
	$(CC) $(COMP_FILES) -o $(BIN_NAME) $(LDFLAGS)

clean:
	rm $(BIN_NAME)

run: $(BIN_NAME)
	./$(BIN_NAME)
