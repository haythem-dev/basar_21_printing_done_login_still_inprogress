
#include "libbasardebug.h"
#include <iostream>

using namespace basar::debug;

int main(int argc, char* argv[])
{
    try {
        std::cout << "=========================================" << std::endl;
        std::cout << "  Basar Regression Test: leak_check_test" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "" << std::endl;
        
        // Test 1: Enable Leak Check
        std::cout << "Test 1: Enable Leak Check" << std::endl;
        enableLeakCheck(true);
        std::cout << "  Leak check enabled" << std::endl;
        
        // Test 2: Disable Leak Check
        std::cout << "" << std::endl;
        std::cout << "Test 2: Disable Leak Check" << std::endl;
        enableLeakCheck(false);
        std::cout << "  Leak check disabled" << std::endl;
        
        // Test 3: Ignore Memory Diff
        std::cout << "" << std::endl;
        std::cout << "Test 3: Ignore Memory Diff" << std::endl;
        ignoreMemoryDiff(10, 1024);
        std::cout << "  Memory diff ignored: 10 blocks, 1024 bytes" << std::endl;
        
        // Test 4: Print Debug Message
        std::cout << "" << std::endl;
        std::cout << "Test 4: Print Debug Message" << std::endl;
        printDbgMsg("Test debug message from regression test");
        std::cout << "  Debug message printed" << std::endl;
        
        // Test 5: Build Server Check
        std::cout << "" << std::endl;
        std::cout << "Test 5: Build Server Check" << std::endl;
        bool isBuild = isBuildServer();
        std::cout << "  Is build server: " << (isBuild ? "yes" : "no") << std::endl;
        
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
