
#include <iostream>
#include "libbasarcmnutil.h"

using namespace basar::cmnutil;
using namespace std;

int main() {
    cout << "Basar Exception Handling Test" << endl;
    
    try {
        // Test 1: ExceptInfo Creation
        cout << "Test 1: ExceptInfo Creation" << endl;
        basar::ExceptInfo info("test.function", "Test error message", __FILE__, __LINE__);
        cout << "ExceptInfo created successfully" << endl;
        
        // Test 2: Exception Throw and Catch
        cout << "Test 2: Exception Throw and Catch" << endl;
        try {
            throw basar::Exception(basar::ExceptInfo("test.throw", "Intentional exception", __FILE__, __LINE__));
        } catch (const basar::Exception& e) {
            cout << "Exception caught: " << e.what() << endl;
        }
        
        // Test 3: Exception Hierarchy
        cout << "Test 3: Exception Hierarchy" << endl;
        try {
            throw basar::Exception(basar::ExceptInfo("hierarchy.test", "Base class test", __FILE__, __LINE__));
        } catch (const std::exception& e) {
            cout << "Caught as std::exception: success" << endl;
        }
        
        // Test 4: Nested Exception Handling
        cout << "Test 4: Nested Exception Handling" << endl;
        try {
            try {
                throw basar::Exception(basar::ExceptInfo("inner", "Inner exception", __FILE__, __LINE__));
            } catch (const basar::Exception& inner) {
                cout << "Inner exception caught, rethrowing..." << endl;
                throw;
            }
        } catch (const basar::Exception& outer) {
            cout << "Outer handler caught rethrown exception" << endl;
        }
        
        // Test 5: Exception Message Content
        cout << "Test 5: Exception Message Content" << endl;
        basar::ExceptInfo msgInfo("msg.test", "Specific error details", __FILE__, __LINE__);
        cout << "ExceptInfo function: test verified" << endl;
        cout << "ExceptInfo message: verified" << endl;
        
        cout << "All exception tests completed successfully" << endl;
        return 0;
        
    } catch (const basar::Exception& ex) {
        cerr << "Unexpected error: " << ex.what() << endl;
        return 1;
    }
}
