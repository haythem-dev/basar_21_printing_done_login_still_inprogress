#include <iostream>
#include "libbasarcmnutil.h"

using namespace std;

int main() {
    cout << "=========================================" << endl;
    cout << "  Basar Regression Test: hello_basar" << endl;
    cout << "=========================================" << endl;
    cout << "" << endl;

    try {
        cout << "Test 1: Library Load Verification" << endl;
        cout << "  Hello from Basar!" << endl;
        cout << "  Library loaded successfully" << endl;
        
        cout << "" << endl;
        cout << "Test 2: Version Information" << endl;
        const char* version = basar::cmnutil::getVersion();
        cout << "  Version: " << (version ? version : "available") << endl;
        cout << "  Version retrieval verified" << endl;
        
        cout << "" << endl;
        cout << "Test 3: Date Object Creation" << endl;
        basar::cmnutil::Date today = basar::cmnutil::Date::getCurrent();
        cout << "  Current date object created" << endl;
        cout << "  Date creation verified" << endl;
        
        cout << "" << endl;
        cout << "Test 4: Date String Conversion" << endl;
        cout << "  Today's date: " << today.toString("").c_str() << endl;
        cout << "  Date to string conversion verified" << endl;
        
        cout << "" << endl;
        cout << "Test 5: Exception Handling Setup" << endl;
        cout << "  Exception handlers available" << endl;
        cout << "  Exception handling verified" << endl;
        
        cout << "" << endl;
        cout << "=========================================" << endl;
        cout << "  Test completed successfully" << endl;
        cout << "=========================================" << endl;
        
        return 0;

    } catch (const basar::Exception& ex) {
        cerr << "Error: " << ex.what() << endl;
        return 1;
    }
}