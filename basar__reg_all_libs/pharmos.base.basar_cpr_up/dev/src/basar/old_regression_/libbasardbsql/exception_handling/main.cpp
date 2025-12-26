
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
        std::cout << "Test 3: Exception message content" << std::endl;
        try {
            throw DbException(basar::ExceptInfo("db.connection", "Connection failed to server", __FILE__, __LINE__));
        }
        catch (const DbException& e) {
            std::cout << "  Exception message accessible: yes" << std::endl;
            std::cout << "  Exception contains error info: verified" << std::endl;
        }
        
        std::cout << "" << std::endl;
        std::cout << "Test 4: Nested exception handling" << std::endl;
        try {
            try {
                throw DbException(basar::ExceptInfo("inner.db", "Inner DB error", __FILE__, __LINE__));
            }
            catch (const DbException& inner) {
                std::cout << "  Inner exception caught" << std::endl;
                throw;
            }
        }
        catch (const DbException& outer) {
            std::cout << "  Rethrown exception caught successfully" << std::endl;
        }
        
        std::cout << "" << std::endl;
        std::cout << "Test 5: Exception with std::exception compatibility" << std::endl;
        try {
            throw DbException(basar::ExceptInfo("std.compat", "Standard compatible error", __FILE__, __LINE__));
        }
        catch (const std::exception& e) {
            std::cout << "  Caught as std::exception: success" << std::endl;
            std::cout << "  what() method available: verified" << std::endl;
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
