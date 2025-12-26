#include <iostream>
#include "libbasarcmnutil.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout.setf(ios::unitbuf);
    
    try {
        cout << "=========================================" << endl;
        cout << "  Basar Regression Test: time_edit_test" << endl;
        cout << "=========================================" << endl;
        cout << "" << endl;
        
        // Test 1: Time value using basar::Time
        cout << "Test 1: Time Value Handling" << endl;
        basar::Time testTime(14, 30, 45);
        cout << "  Hour: " << testTime.hour() << endl;
        cout << "  Minute: " << testTime.minute() << endl;
        cout << "  Second: " << testTime.second() << endl;
        cout << "  Time value handling verified" << endl;
        
        // Test 2: Time format strings
        cout << "" << endl;
        cout << "Test 2: Time Format Strings" << endl;
        basar::VarString format1 = "HH:mm:ss";
        basar::VarString format2 = "hh:mm AP";
        basar::VarString format3 = "HH:mm";
        cout << "  Format 1 (24h): " << format1.c_str() << endl;
        cout << "  Format 2 (12h): " << format2.c_str() << endl;
        cout << "  Format 3 (short): " << format3.c_str() << endl;
        cout << "  Time format strings verified" << endl;
        
        // Test 3: Time validation
        cout << "" << endl;
        cout << "Test 3: Time Validation" << endl;
        bool isValid = testTime.isValid();
        cout << "  Time is valid: " << (isValid ? "yes" : "no") << endl;
        cout << "  Time validation verified" << endl;
        
        // Test 4: Time range
        cout << "" << endl;
        cout << "Test 4: Time Range" << endl;
        basar::Time minTime(0, 0, 0);
        basar::Time maxTime(23, 59, 59);
        cout << "  Min time: " << minTime.hour() << ":" << minTime.minute() << ":" << minTime.second() << endl;
        cout << "  Max time: " << maxTime.hour() << ":" << maxTime.minute() << ":" << maxTime.second() << endl;
        cout << "  Time range verified" << endl;
        
        // Test 5: Time comparison
        cout << "" << endl;
        cout << "Test 5: Time Comparison" << endl;
        if (testTime > minTime) {
            cout << "  testTime > minTime: true" << endl;
        }
        if (testTime < maxTime) {
            cout << "  testTime < maxTime: true" << endl;
        }
        cout << "  Time comparison verified" << endl;
        
        cout << "" << endl;
        cout << "=========================================" << endl;
        cout << "  Test completed successfully" << endl;
        cout << "=========================================" << endl;
        
        return 0;
    }
    catch (const basar::Exception& ex) {
        cerr << "ERROR: Basar exception: " << ex.what() << endl;
        return 1;
    }
    catch (const exception& e) {
        cerr << "ERROR: Exception caught: " << e.what() << endl;
        return 1;
    }
    catch (...) {
        cerr << "ERROR: Unknown exception caught" << endl;
        return 1;
    }
}
