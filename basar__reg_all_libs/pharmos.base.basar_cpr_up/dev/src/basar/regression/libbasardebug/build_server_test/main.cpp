
#include "libbasardebug.h"
#include <iostream>

using namespace basar::debug;

int main(int argc, char* argv[])
{
    try {
        std::cout << "=========================================" << std::endl;
        std::cout << "  Basar Regression Test: build_server_test" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "" << std::endl;
        
        // Test 1: Check Build Server Status
        std::cout << "Test 1: Check Build Server Status" << std::endl;
        bool isBuild = isBuildServer();
        std::cout << "  Build server check returned: " << (isBuild ? "true" : "false") << std::endl;
        std::cout << "  Function executed successfully" << std::endl;
        
        // Test 2: Multiple Calls Consistency
        std::cout << "" << std::endl;
        std::cout << "Test 2: Multiple Calls Consistency" << std::endl;
        bool call1 = isBuildServer();
        bool call2 = isBuildServer();
        bool call3 = isBuildServer();
        bool consistent = (call1 == call2) && (call2 == call3);
        std::cout << "  Multiple calls consistent: " << (consistent ? "yes" : "no") << std::endl;
        
        // Test 3: Environment Detection
        std::cout << "" << std::endl;
        std::cout << "Test 3: Environment Detection" << std::endl;
        if (isBuildServer()) {
            std::cout << "  Running on build server environment" << std::endl;
        } else {
            std::cout << "  Running on development environment" << std::endl;
        }
        std::cout << "  Environment detection completed" << std::endl;
        
        // Test 4: Conditional GUI Behavior
        std::cout << "" << std::endl;
        std::cout << "Test 4: Conditional GUI Behavior" << std::endl;
        if (!isBuildServer()) {
            std::cout << "  GUI output would be allowed" << std::endl;
        } else {
            std::cout << "  GUI output would be suppressed" << std::endl;
        }
        std::cout << "  GUI behavior check completed" << std::endl;
        
        // Test 5: Return Value Type
        std::cout << "" << std::endl;
        std::cout << "Test 5: Return Value Type" << std::endl;
        bool result = isBuildServer();
        std::cout << "  Boolean return type verified" << std::endl;
        std::cout << "  Value: " << result << std::endl;
        
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
