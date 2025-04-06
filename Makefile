PLATFORM ?= Windows
# Linux or Mac

COMP_FILES = src/main.c src/player.c src/level.c src/animation.c src/objects.c
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
	$(CC) $(COMP_FILES) -o $(BIN_NAME) $(LDFLAGS) -lm -Iinclude -DGRAPHICS_API_OPENGL_100

RAYLIB := $(HOME)/pkg/raylib/src/
web: $(COMP_FILES)
	emcc -o web/$(BIN_NAME).html $(COMP_FILES) -Os -Wall $(RAYLIB)libraylib.web.a -I. -I$(RAYLIB) -L. -L$(RAYLIB)libraylib.web.a -lm -Iinclude -s USE_GLFW=3 -s ASYNCIFY -DPLATFORM_WEB --preload-file assets -DGRAPHICS_API_OPENGL_100 --shell-file web/game.html

clean:
	@rm -f $(BIN_NAME) web/$(BIN_NAME).data web/$(BIN_NAME).js web/$(BIN_NAME).wasm

run: $(BIN_NAME)
	@./$(BIN_NAME)

.PHONY: clean run web
