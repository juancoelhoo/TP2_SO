CC = gcc
CFLAGS = -Iinclude -Wall
SRC = src
OBJ = obj
BIN = bin
RES = resultados

SOURCES = $(wildcard $(SRC)/*.c)
OBJECTS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
EXEC = $(BIN)/simulador

all: $(EXEC)

$(EXEC): $(OBJECTS) | $(BIN)
	$(CC) $(OBJECTS) -o $@

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ):
	mkdir -p $(OBJ)

$(BIN):
	mkdir -p $(BIN)

clean:
	rm -rf $(OBJ)/*.o $(EXEC) $(RES)/*.txt

.PHONY: all clean
