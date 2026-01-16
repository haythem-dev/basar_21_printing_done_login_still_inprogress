# Makefile for 02_login_types

CC = g++
CFLAGS = -std=c++17 -Wall -Wextra
TARGET = test_02_login_types
SOURCES = main.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: all run clean
