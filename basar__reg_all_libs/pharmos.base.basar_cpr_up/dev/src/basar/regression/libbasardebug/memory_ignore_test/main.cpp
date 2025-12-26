
#include "libbasardebug.h"
#include <iostream>

using namespace basar::debug;

int main(int argc, char* argv[])
{
    try {
        std::cout << "=========================================" << std::endl;
        std::cout << "  Basar Regression Test: memory_ignore_test" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "" << std::endl;
        
        // Test 1: Zero Ignore
        std::cout << "Test 1: Zero Ignore" << std::endl;
        ignoreMemoryDiff(0, 0);
        std::cout << "  Zero ignore values set" << std::endl;
        
        // Test 2: Small Ignore
        std::cout << "" << std::endl;
        std::cout << "Test 2: Small Ignore" << std::endl;
        ignoreMemoryDiff(1, 64);
        std::cout << "  Small ignore values set: 1 block, 64 bytes" << std::endl;
        
        // Test 3: Medium Ignore
        std::cout << "" << std::endl;
        std::cout << "Test 3: Medium Ignore" << std::endl;
        ignoreMemoryDiff(100, 4096);
        std::cout << "  Medium ignore values set: 100 blocks, 4096 bytes" << std::endl;
        
        // Test 4: Large Ignore
        std::cout << "" << std::endl;
        std::cout << "Test 4: Large Ignore" << std::endl;
        ignoreMemoryDiff(10000, 1048576);
        std::cout << "  Large ignore values set: 10000 blocks, 1MB" << std::endl;
        
        // Test 5: Reset to Default
        std::cout << "" << std::endl;
        std::cout << "Test 5: Reset to Default" << std::endl;
        ignoreMemoryDiff(0, 0);
        std::cout << "  Reset to default ignore values" << std::endl;
        
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
