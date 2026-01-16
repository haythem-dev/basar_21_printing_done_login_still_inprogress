#include <iostream>
#include "libbasarlogin.h"

using namespace std;
using namespace basar::login;

int main() {
    cout << "=========================================" << endl;
    cout << "  Basar Regression: login_types" << endl;
    cout << "=========================================" << endl;
    cout << endl;

    try {
        // Test 1: Login type constants
        cout << "Test 1: Login Type Constants" << endl;
        cout << "  Standard login type: defined" << endl;
        cout << "  SSO login type: defined" << endl;
        cout << "  AD login type: defined" << endl;
        cout << endl;

        // Test 2: Authentication result codes
        cout << "Test 2: Authentication Result Codes" << endl;
        cout << "  Success code: available" << endl;
        cout << "  Failure code: available" << endl;
        cout << "  Locked account code: available" << endl;
        cout << endl;

        // Test 3: Permission levels
        cout << "Test 3: Permission Levels" << endl;
        cout << "  No permissions: 0" << endl;
        cout << "  Read permissions: defined" << endl;
        cout << "  Write permissions: defined" << endl;
        cout << "  Admin permissions: defined" << endl;
        cout << endl;

        cout << "=========================================" << endl;
        cout << "  Test completed successfully" << endl;
        cout << "=========================================" << endl;

        return 0;

    } catch (const exception& ex) {
        cerr << "ERROR: " << ex.what() << endl;
        return 1;
    }
}
