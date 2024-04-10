# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall
LDFLAGS = -pthread

# Source, object, binary and tmp directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Application specific vars
APP_NAME = chat-server
EXEC = $(BIN_DIR)/$(APP_NAME)

# Include directory
INCLUDES = -I../include -I../Common/inc
COMMON_OBJ_DIR = ../Common/obj

# All source and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all clean

# Default target builds common and then application
all: common $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(EXEC) $(OBJS) $(wildcard $(COMMON_OBJ_DIR)/*.o) $(LDFLAGS)

# Build common objects
common:
	$(MAKE) -C ../Common

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(EXEC)