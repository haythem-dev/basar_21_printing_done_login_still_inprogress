#include <iostream>
#include "libbasarcmnutil.h"

using namespace std;

int main() {
    cout << "Hello from Basar!" << endl;

    try {
        // Simple test - create a Date object to verify library works
        basar::cmnutil::Date today = basar::cmnutil::Date::getCurrent();
        cout << "Version: Basar Common Utilities loaded successfully" << endl;
        cout << "Today's date: " << today.toString("").c_str() << endl;
        cout << "Test completed successfully" << endl;
        return 0;

    } catch (const basar::Exception& ex) {
        cerr << "Error: " << ex.what() << endl;
        return 1;
    }
}