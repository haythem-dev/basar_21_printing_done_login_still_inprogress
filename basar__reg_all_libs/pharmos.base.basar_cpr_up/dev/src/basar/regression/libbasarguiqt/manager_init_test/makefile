TARGET_NAME = manager_init_test

# Directories
BASAR_LIB_DIR = ../../../library/lib/debug
BASAR_INC_DIR = ../../../library/include
LOG4CPLUS_INC = ../../../../../ext/log4cplus/include
LOG4CPLUS_LIB = ../../../../../ext/log4cplus/lib/debug
BOOST_INC = ../../../../../ext/boost/include
BOOST_LIB = ../../../../../ext/boost/lib
QT5_INC = ../../../../../ext/qt5/include
QT5_LIB = ../../../../../ext/qt5/lib

# Libraries
LIBS = -L$(BASAR_LIB_DIR) -L$(LOG4CPLUS_LIB) -L$(BOOST_LIB) -L$(QT5_LIB) \
       -lbasarguiqt -lbasarcmnutil -lbasardebug -llog4cplus \
       -lboost_system -lboost_filesystem

# Include paths
INCS = -I$(BASAR_INC_DIR) -I$(LOG4CPLUS_INC) -I$(QT5_INC) -I$(BOOST_INC)

# Compiler flags
CXXFLAGS = -g -std=c++17 -maix64 -pthread -fPIC

# Linker flags
LDFLAGS = -maix64 -pthread -Wl,-brtl

all: $(TARGET_NAME)

$(TARGET_NAME): main.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $< $(LIBS) $(LDFLAGS)

clean:
	rm -f $(TARGET_NAME) *.o go.out
