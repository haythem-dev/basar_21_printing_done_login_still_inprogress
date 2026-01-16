# Makefile for 04_authentication_states

CC = g++
CFLAGS = -std=c++17 -Wall -Wextra
TARGET = test_04_authentication_states
SOURCES = main.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: all run clean
