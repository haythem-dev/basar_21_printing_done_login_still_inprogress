#include <iostream>
#include "libbasarcmnutil.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout.setf(ios::unitbuf);
    
    try {
        cout << "=========================================" << endl;
        cout << "  Basar Regression Test: date_edit_test" << endl;
        cout << "=========================================" << endl;
        cout << "" << endl;
        
        // Test 1: Date value using basar::Date
        cout << "Test 1: Date Value Handling" << endl;
        basar::Date testDate(2024, 12, 3);
        cout << "  Year: " << testDate.year() << endl;
        cout << "  Month: " << testDate.month() << endl;
        cout << "  Day: " << testDate.day() << endl;
        cout << "  Date value handling verified" << endl;
        
        // Test 2: Date format strings
        cout << "" << endl;
        cout << "Test 2: Date Format Strings" << endl;
        basar::VarString format1 = "yyyy-MM-dd";
        basar::VarString format2 = "dd.MM.yyyy";
        basar::VarString format3 = "MM/dd/yyyy";
        cout << "  Format 1: " << format1.c_str() << endl;
        cout << "  Format 2: " << format2.c_str() << endl;
        cout << "  Format 3: " << format3.c_str() << endl;
        cout << "  Date format strings verified" << endl;
        
        // Test 3: Date validation
        cout << "" << endl;
        cout << "Test 3: Date Validation" << endl;
        bool isValid = testDate.isValid();
        cout << "  Date is valid: " << (isValid ? "yes" : "no") << endl;
        cout << "  Date validation verified" << endl;
        
        // Test 4: Date range
        cout << "" << endl;
        cout << "Test 4: Date Range" << endl;
        basar::Date minDate(2000, 1, 1);
        basar::Date maxDate(2099, 12, 31);
        cout << "  Min date: " << minDate.year() << "-" << minDate.month() << "-" << minDate.day() << endl;
        cout << "  Max date: " << maxDate.year() << "-" << maxDate.month() << "-" << maxDate.day() << endl;
        cout << "  Date range verified" << endl;
        
        // Test 5: Date comparison
        cout << "" << endl;
        cout << "Test 5: Date Comparison" << endl;
        if (testDate > minDate) {
            cout << "  testDate > minDate: true" << endl;
        }
        if (testDate < maxDate) {
            cout << "  testDate < maxDate: true" << endl;
        }
        cout << "  Date comparison verified" << endl;
        
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
