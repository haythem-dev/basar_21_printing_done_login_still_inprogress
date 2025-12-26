
#include <libbasardbsql.h>
#include <iostream>

using namespace basar::db::sql;

int main(int argc, char* argv[])
{
    std::cout.setf(std::ios::unitbuf);  // Force unbuffered output
    
    try {
        std::cout << "=========================================" << std::endl;
        std::cout.flush();
        std::cout << "  Basar Regression Test: connection_basic" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "" << std::endl;
        
        // Create database info (mock - no actual connection needed for structure test)
        DatabaseInfo dbInfo;
        std::cout << "DatabaseInfo object created successfully" << std::endl;
        
        // Test version information
        std::cout << "libbasardbsql version: " << getVersion() << std::endl;
        
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
