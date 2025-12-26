TARGET_NAME = threading_test

# Compiler flags
CXXFLAGS = -g -std=c++17 -maix64 -pthread

# Linker flags
LDFLAGS = -maix64 -pthread

all: $(TARGET_NAME)

$(TARGET_NAME): main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGET_NAME) *.o go.out
