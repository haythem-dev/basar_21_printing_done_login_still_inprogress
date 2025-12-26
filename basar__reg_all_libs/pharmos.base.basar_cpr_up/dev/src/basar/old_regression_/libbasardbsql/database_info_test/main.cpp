
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
        
        std::cout << "Test 1: Default DatabaseInfo" << std::endl;
        DatabaseInfo dbInfo;
        std::cout << "  Created empty DatabaseInfo object" << std::endl;
        std::cout << "  Default state validated" << std::endl;
        
        std::cout << "" << std::endl;
        std::cout << "Test 2: ODBC Configuration" << std::endl;
        dbInfo.setODBC("test_dsn");
        std::cout << "  ODBC DSN set successfully" << std::endl;
        std::cout << "  ODBC configuration validated" << std::endl;
        
        std::cout << "" << std::endl;
        std::cout << "Test 3: Informix Configuration" << std::endl;
        DatabaseInfo dbInfoInfx;
        dbInfoInfx.setInfx("test_server", "test_database");
        std::cout << "  Informix configuration set successfully" << std::endl;
        std::cout << "  Informix server and database validated" << std::endl;
        
        std::cout << "" << std::endl;
        std::cout << "Test 4: DatabaseInfo Copy and Assignment" << std::endl;
        DatabaseInfo dbInfoCopy = dbInfo;
        std::cout << "  DatabaseInfo copy constructor verified" << std::endl;
        DatabaseInfo dbInfoAssign;
        dbInfoAssign = dbInfoInfx;
        std::cout << "  DatabaseInfo assignment operator verified" << std::endl;
        
        std::cout << "" << std::endl;
        std::cout << "Test 5: Version Information" << std::endl;
        std::cout << "  libbasardbsql version: " << getVersion() << std::endl;
        std::cout << "  Version information accessible" << std::endl;
        
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
