# Makefile for 03_configuration_basic

CC = g++
CFLAGS = -std=c++17 -Wall -Wextra
TARGET = test_03_configuration_basic
SOURCES = main.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: all run clean
