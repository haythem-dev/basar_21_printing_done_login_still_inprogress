
CXX = g++
CXXFLAGS = -g -std=c++17 -maix64
TARGET_NAME = cpp_basics

# Linker flags for AIX
LDFLAGS = -maix64

all: $(TARGET_NAME)

$(TARGET_NAME): main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGET_NAME) *.o go.out

.PHONY: all clean
