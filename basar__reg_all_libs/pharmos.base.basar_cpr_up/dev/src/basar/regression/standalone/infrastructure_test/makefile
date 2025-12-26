##############################################################
#                                                            #
#   Basar Regression Test: infrastructure_test              #
#                                                            #
##############################################################

TARGET_NAME = infrastructure_test

# Compiler flags
CXXFLAGS = -g -std=c++17 -maix64

# Linker flags
LDFLAGS = -maix64

all: $(TARGET_NAME)

infrastructure_test: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGET_NAME) *.o go.out

.PHONY: all clean