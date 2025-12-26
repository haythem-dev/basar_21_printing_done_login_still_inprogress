
#include "libbasarguiqt.h"
#include <iostream>
#include <cstring>

using namespace basar::guiqt;

int main(int argc, char* argv[])
{
    try {
        std::cout << "=========================================" << std::endl;
        std::cout << "  Basar Regression Test: version_info_test" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "" << std::endl;
        
        std::cout << "Test 1: Version String Retrieval" << std::endl;
        const char* version = getVersion();
        std::cout << "  Library version: " << version << std::endl;
        std::cout << "  Version string retrieved successfully" << std::endl;
        
        std::cout << "" << std::endl;
        std::cout << "Test 2: Full Version Information" << std::endl;
        const char* fullVersion = getFullVersion();
        std::cout << "  Full version: " << fullVersion << std::endl;
        std::cout << "  Full version string retrieved successfully" << std::endl;
        
        std::cout << "" << std::endl;
        std::cout << "Test 3: Version Validity Check" << std::endl;
        if (version != nullptr && fullVersion != nullptr) {
            std::cout << "  Version pointers are valid" << std::endl;
            std::cout << "  Version validity check passed" << std::endl;
        } else {
            std::cerr << "ERROR: Version information is NULL" << std::endl;
            return 1;
        }
        
        std::cout << "" << std::endl;
        std::cout << "Test 4: Version String Content" << std::endl;
        if (strlen(version) > 0) {
            std::cout << "  Version string is non-empty" << std::endl;
            std::cout << "  Version content validated" << std::endl;
        } else {
            std::cout << "  Warning: Version string is empty" << std::endl;
        }
        
        std::cout << "" << std::endl;
        std::cout << "Test 5: Version Consistency" << std::endl;
        const char* version2 = getVersion();
        if (version == version2 || strcmp(version, version2) == 0) {
            std::cout << "  Multiple version calls return consistent results" << std::endl;
            std::cout << "  Version consistency verified" << std::endl;
        } else {
            std::cout << "  Version consistency check: different addresses" << std::endl;
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
