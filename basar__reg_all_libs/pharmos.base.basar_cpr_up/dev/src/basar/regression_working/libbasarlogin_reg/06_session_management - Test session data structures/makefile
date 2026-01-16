# Makefile for 06_session_management

CC = g++
CFLAGS = -std=c++17 -Wall -Wextra
TARGET = test_06_session_management
SOURCES = main.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: all run clean
