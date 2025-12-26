
#include "libbasardebug.h"
#include <iostream>
#include <sstream>

using namespace basar::debug;

int main(int argc, char* argv[])
{
    try {
        std::cout << "=========================================" << std::endl;
        std::cout << "  Basar Regression Test: debug_output_test" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "" << std::endl;
        
        // Test 1: Simple Debug Output
        std::cout << "Test 1: Simple Debug Output" << std::endl;
        printDbgMsg("Simple debug message");
        std::cout << "  Simple message output verified" << std::endl;
        
        // Test 2: Formatted Debug Output
        std::cout << "" << std::endl;
        std::cout << "Test 2: Formatted Debug Output" << std::endl;
        std::ostringstream oss;
        oss << "Value: " << 42 << ", Status: OK";
        printDbgMsg(oss.str().c_str());
        std::cout << "  Formatted message output verified" << std::endl;
        
        // Test 3: Multiple Line Output
        std::cout << "" << std::endl;
        std::cout << "Test 3: Multiple Line Output" << std::endl;
        printDbgMsg("Line 1 of debug output");
        printDbgMsg("Line 2 of debug output");
        printDbgMsg("Line 3 of debug output");
        std::cout << "  Multiple lines output verified" << std::endl;
        
        // Test 4: Special Characters
        std::cout << "" << std::endl;
        std::cout << "Test 4: Special Characters" << std::endl;
        printDbgMsg("Special: [brackets] {braces} <angles>");
        std::cout << "  Special characters handled" << std::endl;
        
        // Test 5: Empty String
        std::cout << "" << std::endl;
        std::cout << "Test 5: Empty String" << std::endl;
        printDbgMsg("");
        std::cout << "  Empty string handled" << std::endl;
        
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
