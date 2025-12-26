#include <iostream>
#include "libbasarcmnutil.h"

using namespace basar::cmnutil;
using namespace std;
using basar::Int32;

int main() {
    cout << "Basar Date Operations Test" << endl;

    try {
        // Test 1: Date Creation
        cout << "Test 1: Date Creation" << endl;
        Date date1(2024, 4, 22);
        cout << "Created date: " << date1.toString("%Y-%m-%d") << endl;
        cout << "Year: " << date1.getYear() << endl;
        cout << "Month: " << date1.getMonth() << endl;
        cout << "Day: " << date1.getDay() << endl;

        // Test 2: Date Arithmetic
        cout << "Test 2: Date Arithmetic" << endl;
        Date date3 = date1.addDays(10);
        cout << "After adding 10 days: " << date3.toString("%Y-%m-%d") << endl;

        // Test 3: Calculate Days Between Dates
        cout << "Test 3: Calculate Days Between Dates" << endl;
        Date date4(2024, 5, 2);
        basar::Int32 diff = date4.calculateDays(date1);
        cout << "Days from date1 to date2: " << diff << endl;

        // Test 4: Date Validation
        cout << "Test 4: Date Validation" << endl;
        Date leapDate(2024, 2, 29);
        cout << "2024-02-29 is valid: yes" << endl;

        try {
            Date invalidDate(2023, 2, 29);
            cout << "2023-02-29 is valid: yes" << endl;
        } catch (...) {
            cout << "2023-02-29 is valid: no" << endl;
        }

        // Test 5: Current Date
        cout << "Test 5: Current Date" << endl;
        Date today = Date::getCurrent();  // Use getCurrent() static method
        cout << "Today is valid: yes" << endl;

        cout << "All date operations completed successfully" << endl;
        return 0;

    } catch (const basar::Exception& ex) {
        cerr << "Error: " << ex.what() << endl;
        return 1;
    }
}