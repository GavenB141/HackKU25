LDFLAGS := -lraylib

BIN_NAME := game

$(BIN_NAME): src/main.c
	gcc src/main.c -o $(BIN_NAME) $(LDFLAGS)

clean:
	rm $(BIN_NAME)

run: $(BIN_NAME)
	./$(BIN_NAME)
