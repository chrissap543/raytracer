# Current type of implementation
IMPL = CPU

# Compiler options
CC = g++

# Project structure
SRC_DIR = src
OBJ_DIR = bin
INC_DIR = include

EXEC = ray-tracing
OBJS = $(OBJ_DIR)/main.o

# Compiling

all: $(EXEC)

$(EXEC) : $(OBJS)
	$(CC) -o $(EXEC) $(OBJS)

$(OBJ_DIR)/main.o : $(SRC_DIR)/main.cc $(wildcard $(INC_DIR)/*.h) | $(OBJ_DIR)
	$(CC) -I$(INC_DIR) -o $@ -c $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(EXEC) $(OBJ_DIR)
