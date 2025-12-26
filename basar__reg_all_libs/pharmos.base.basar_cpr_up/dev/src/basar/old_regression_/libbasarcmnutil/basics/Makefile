
# Makefile for libbasarcmnutil basics test

CXX = /opt/freeware/bin/g++-12
CXXFLAGS = -I../../../../libbasarcmnutil \
           -I../../../../../ext/unittestpp/include \
           -I../../../../../ext/log4cplus/include \
           -I../../../../../ext/boost/include \
           -fPIC -std=c++17 -Wl,-brtl -maix64 -lpthread -lpthreads -pthread

LDFLAGS = -L../../../../../ext/unittestpp/lib/Release \
          -L../../../../../ext/zlib/lib/Release \
          -L/informix/informix11.70FC5/lib \
          -L/informix/informix11.70FC5/lib/esql

LIBS = -lbasarcmnutil -lbasardebug -lUnitTest++

TARGET = test_basar
SOURCES = test_basar.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)

clean:
	rm -f $(TARGET) *.o

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
