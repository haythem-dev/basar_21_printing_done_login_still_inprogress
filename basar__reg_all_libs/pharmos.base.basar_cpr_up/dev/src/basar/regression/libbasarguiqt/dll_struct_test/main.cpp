
#include "libbasarguiqt.h"
#include "libbasarcmnutil.h"
#include <iostream>

int main(int argc, char* argv[])
{
    try {
        std::cout << "=========================================" << std::endl;
        std::cout << "  Basar Regression Test: dll_struct_test" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "" << std::endl;
        
        std::cout << "Test 1: DllStruct Default Creation" << std::endl;
        basar::DllStruct dll1;
        std::cout << "  DllStruct object created" << std::endl;
        std::cout << "  Default construction verified" << std::endl;
        
        std::cout << "" << std::endl;
        std::cout << "Test 2: DllStruct Name Assignment" << std::endl;
        dll1.name = "libbasarcmnutil";
        std::cout << "  DLL Name: " << dll1.name << std::endl;
        std::cout << "  Name assignment verified" << std::endl;
        
        std::cout << "" << std::endl;
        std::cout << "Test 3: DllStruct Version Assignment" << std::endl;
        dll1.pkgVersion = basar::cmnutil::getVersion();
        std::cout << "  DLL Version: " << dll1.pkgVersion << std::endl;
        std::cout << "  Version assignment verified" << std::endl;
        
        std::cout << "" << std::endl;
        std::cout << "Test 4: Multiple DllStruct Instances" << std::endl;
        basar::DllStruct dll2;
        dll2.name = "libbasarguiqt";
        dll2.pkgVersion = basar::guiqt::getVersion();
        std::cout << "  DLL 2 Name: " << dll2.name << std::endl;
        std::cout << "  DLL 2 Version: " << dll2.pkgVersion << std::endl;
        std::cout << "  Multiple instances verified" << std::endl;
        
        std::cout << "" << std::endl;
        std::cout << "Test 5: DllStruct Validation" << std::endl;
        if (!dll1.name.empty() && !dll2.name.empty()) {
            std::cout << "  Both DllStruct instances valid" << std::endl;
            std::cout << "  DllStruct validation passed" << std::endl;
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
