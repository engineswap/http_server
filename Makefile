# Makefile for a multi-file C++ project with unit tests

# Variables
CC = g++
CFLAGS = -std=c++17
SRC = ./src/main.cpp ./src/http_server.cpp 
OBJ = $(SRC:.cpp=.o)
TARGET = server 

# Default target builds both main program and unit tests
all: $(TARGET) 

# Linking the main executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

# Compiling source files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)

# Phony target to prevent conflicts with file names
.PHONY: clean
