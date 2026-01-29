CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TARGET = $(BIN_DIR)/graph_model

all: directories $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"

clean:
	@if exist "$(OBJ_DIR)" rmdir /s /q "$(OBJ_DIR)"
	@if exist "$(BIN_DIR)" rmdir /s /q "$(BIN_DIR)"

run: all
	./$(TARGET)

.PHONY: all clean run directories
