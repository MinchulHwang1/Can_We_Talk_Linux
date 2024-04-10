# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall

# Source, object, and includes directories
SRC_DIR = src
OBJ_DIR = obj
INCLUDES = -Iinc

# All source and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o

