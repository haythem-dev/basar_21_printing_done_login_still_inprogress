
#include <libbasardbsql.h>
#include <iostream>

using namespace basar::db::sql;

int main(int argc, char* argv[])
{
    try {
        std::cout << "=========================================" << std::endl;
        std::cout << "  Basar Regression Test: exception_handling" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "" << std::endl;
        
        std::cout << "Test 1: DbException construction" << std::endl;
        try {
            throw DbException(basar::ExceptInfo("test.function", "Test error", __FILE__, __LINE__));
        }
        catch (const DbException& e) {
            std::cout << "  DbException caught successfully" << std::endl;
        }
        
        std::cout << "" << std::endl;
        std::cout << "Test 2: Exception hierarchy" << std::endl;
        try {
            throw DbException(basar::ExceptInfo("test.function", "Test error", __FILE__, __LINE__));
        }
        catch (const basar::Exception& e) {
            std::cout << "  Exception caught as base class" << std::endl;
        }
        
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
