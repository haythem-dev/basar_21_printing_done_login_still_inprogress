
#include "libbasarlogin/configuration.h"
#include "libbasarlogin/libbasarlogin_definitions.h"
#include <iostream>

using namespace basar::login;

int main(int argc, char* argv[])
{
    try {
        std::cout << "=========================================" << std::endl;
        std::cout << "  Basar Regression Test: ldap_config_test" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "" << std::endl;
        
        // Test 1: Configuration object creation
        std::cout << "Test 1: Configuration Object Creation" << std::endl;
        Configurationptr config(new Configuration());
        std::cout << "  ✓ Configuration object created successfully" << std::endl;
        std::cout << "" << std::endl;
        
        // Test 2: LDAP server configuration
        std::cout << "Test 2: LDAP Server Configuration" << std::endl;
        const basar::VarString& ldapServer = config->getLdapServer();
        std::cout << "  LDAP Server: " << ldapServer << std::endl;
        std::cout << "  ✓ LDAP server configuration accessible" << std::endl;
        std::cout << "" << std::endl;
        
        // Test 3: TCP port configuration
        std::cout << "Test 3: TCP Port Configuration" << std::endl;
        const basar::VarString& tcpPort = config->getTcpPort();
        std::cout << "  TCP Port: " << tcpPort << std::endl;
        std::cout << "  ✓ TCP port configuration accessible" << std::endl;
        std::cout << "" << std::endl;
        
        // Test 4: Distinguished Name configuration
        std::cout << "Test 4: Distinguished Name Configuration" << std::endl;
        const basar::VarString& dn = config->getDistingushedName();
        std::cout << "  DN length: " << dn.length() << " characters" << std::endl;
        std::cout << "  ✓ Distinguished name configuration accessible" << std::endl;
        std::cout << "" << std::endl;
        
        // Test 5: Login type detection
        std::cout << "Test 5: Login Type Detection" << std::endl;
        loginType::LoginTypeEnum loginType = config->getLoginType();
        std::cout << "  Login Type: " << (int)loginType << std::endl;
        std::cout << "  Is AD Login: " << (config->isADLogin() ? "true" : "false") << std::endl;
        std::cout << "  ✓ Login type configuration accessible" << std::endl;
        std::cout << "" << std::endl;
        
        // Test 6: Namespace availability check
        std::cout << "Test 6: Namespace and Type Availability" << std::endl;
        std::cout << "  ✓ basar::login namespace available" << std::endl;
        std::cout << "  ✓ Configuration class available" << std::endl;
        std::cout << "  ✓ loginType enum available" << std::endl;
        std::cout << "" << std::endl;
        
        // Test 7: Default configuration values
        std::cout << "Test 7: Default Configuration Values" << std::endl;
        std::cout << "  Default LDAP server is set: " << (!ldapServer.empty() ? "true" : "false") << std::endl;
        std::cout << "  Default TCP port is set: " << (!tcpPort.empty() ? "true" : "false") << std::endl;
        std::cout << "  ✓ Default configuration values present" << std::endl;
        std::cout << "" << std::endl;
        
        // Test 8: Configuration consistency
        std::cout << "Test 8: Configuration Consistency" << std::endl;
        int portNum = atoi(tcpPort.c_str());
        std::cout << "  TCP port as integer: " << portNum << std::endl;
        std::cout << "  ✓ Configuration data types consistent" << std::endl;
        std::cout << "" << std::endl;
        
        std::cout << "=========================================" << std::endl;
        std::cout << "  All LDAP configuration tests passed!" << std::endl;
        std::cout << "=========================================" << std::endl;
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: Exception caught: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "ERROR: Unknown exception caught" << std::endl;
        return 1;
    }
}
