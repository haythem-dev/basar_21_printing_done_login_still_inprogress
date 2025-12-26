
#include "libbasardebug.h"
#include <iostream>

using namespace basar::debug;

int main(int argc, char* argv[])
{
    try {
        std::cout << "=========================================" << std::endl;
        std::cout << "  Basar Regression Test: print_error_test" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "" << std::endl;
        
        // Test 1: Print Error with Message
        std::cout << "Test 1: Print Error with Message" << std::endl;
        printError("Test error message", "Test Heading");
        std::cout << "  Error message printed with heading" << std::endl;
        
        // Test 2: Print Error Empty Heading
        std::cout << "" << std::endl;
        std::cout << "Test 2: Print Error Empty Heading" << std::endl;
        printError("Error without heading", "");
        std::cout << "  Error printed without heading" << std::endl;
        
        // Test 3: Print Error Long Message
        std::cout << "" << std::endl;
        std::cout << "Test 3: Print Error Long Message" << std::endl;
        printError("This is a longer error message that spans multiple words to test buffer handling", "Long Test");
        std::cout << "  Long error message printed" << std::endl;
        
        // Test 4: Print Debug Sequence
        std::cout << "" << std::endl;
        std::cout << "Test 4: Print Debug Sequence" << std::endl;
        printDbgMsg("Debug 1");
        printDbgMsg("Debug 2");
        printDbgMsg("Debug 3");
        std::cout << "  Debug sequence printed" << std::endl;
        
        // Test 5: Combined Error and Debug
        std::cout << "" << std::endl;
        std::cout << "Test 5: Combined Error and Debug" << std::endl;
        printDbgMsg("Before error");
        printError("Intermixed error", "Mixed");
        printDbgMsg("After error");
        std::cout << "  Combined output completed" << std::endl;
        
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
