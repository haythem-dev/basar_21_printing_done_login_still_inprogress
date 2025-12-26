
##############################################################
#                                                            #
#   Basar Regression Test: ldap_config_test                 #
#                                                            #
##############################################################

TARGET_NAME = ldap_config_test

# Directories
BASAR_LIB_DIR = ../../../library/lib/debug
BASAR_INC_DIR = ../../../..
LOG4CPLUS_INC = ../../../../../ext/log4cplus/include
LOG4CPLUS_LIB = ../../../../../ext/log4cplus/lib/debug
BOOST_LIB = ../../../../../ext/boost/lib
OPENSSL_LIB = ../../../../../ext/openssl/lib

# Libraries
LIBS = -L$(BASAR_LIB_DIR) -L$(LOG4CPLUS_LIB) -L$(BOOST_LIB) -L$(OPENSSL_LIB) \
       -lbasarlogin -lbasarcmnutil -lbasardebug -llog4cplus \
       -lboost_system -lboost_filesystem -lssl -lcrypto

# Include paths
INCS = -I$(BASAR_INC_DIR) -I$(LOG4CPLUS_INC)

# Compiler flags (AIX 64-bit)
CXXFLAGS = -g -std=c++17 -maix64 -pthread

# Linker flags (AIX runtime linking)
LDFLAGS = -maix64 -pthread -Wl,-brtl

all: $(TARGET_NAME)

$(TARGET_NAME): main.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $< $(LIBS) $(LDFLAGS)

clean:
	rm -f $(TARGET_NAME) *.o go.out

.PHONY: all clean
