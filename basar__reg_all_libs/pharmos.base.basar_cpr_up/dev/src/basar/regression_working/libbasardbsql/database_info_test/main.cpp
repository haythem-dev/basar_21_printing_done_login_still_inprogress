
#include <libbasardbsql.h>
#include <iostream>

using namespace basar::db::sql;

int main(int argc, char* argv[])
{
    try {
        std::cout << "=========================================" << std::endl;
        std::cout << "  Basar Regression Test: database_info_test" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "" << std::endl;
        
        // Test DatabaseInfo configuration methods
        DatabaseInfo dbInfo;
        
        std::cout << "Test 1: Default DatabaseInfo" << std::endl;
        std::cout << "  Created empty DatabaseInfo object" << std::endl;
        
        std::cout << "" << std::endl;
        std::cout << "Test 2: ODBC Configuration" << std::endl;
        dbInfo.setODBC("test_dsn");
        std::cout << "  ODBC DSN set successfully" << std::endl;
        
        std::cout << "" << std::endl;
        std::cout << "Test 3: Informix Configuration" << std::endl;
        DatabaseInfo dbInfoInfx;
        dbInfoInfx.setInfx("test_server", "test_database");
        std::cout << "  Informix configuration set successfully" << std::endl;
        
        std::cout << "" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "  Test completed successfully" << std::endl;
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
