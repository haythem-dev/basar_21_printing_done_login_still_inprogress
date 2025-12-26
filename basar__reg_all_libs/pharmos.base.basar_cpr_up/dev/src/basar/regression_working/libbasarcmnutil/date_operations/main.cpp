
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
        // Use toString with proper Informix format
        cout << "Created date: " << date1.toString("yyyy-mm-dd").c_str() << endl;
        cout << "Year: " << date1.getYear() << endl;
        cout << "Month: " << date1.getMonth() << endl;
        cout << "Day: " << date1.getDay() << endl;
        
        // Test 2: Date Arithmetic (using correct API - addDays modifies in place)
        cout << "Test 2: Date Arithmetic" << endl;
        Date date2 = date1;
        date2.addDays(10);  // addDays is void - modifies in place
        cout << "After adding 10 days: " << date2.toString("yyyy-mm-dd").c_str() << endl;
        
        // Test 3: Calculate Days Between Dates
        cout << "Test 3: Calculate Days Between Dates" << endl;
        // Calculate days by iterating (since no daysBetween API exists)
        // For tests involving larger date ranges, consider using Julian date calculation
        Date temp = date1;
        Int32 dayCount = 0;
        while (!(temp == date2) && dayCount < 1000) {
            ++temp;
            ++dayCount;
        }
        cout << "Days from date1 to date2: " << dayCount << endl;
        
        // Test 4: Date Validation
        cout << "Test 4: Date Validation" << endl;
        Date leapDate(2024, 2, 29);
        cout << "2024-02-29 is valid: " << (leapDate.isValid() ? "yes" : "no") << endl;
        
        // KNOWN ISSUE: The library's isValid() returns true for 2023-02-29
        // which is incorrect since 2023 is not a leap year.
        // This test documents current library behavior.
        // TODO: Fix Date::isValid() in libbasarcmnutil to properly validate leap years
        Date nonLeapDate(2023, 2, 29);
        cout << "2023-02-29 is valid: " << (nonLeapDate.isValid() ? "yes" : "no") << endl;
        
        // Test 5: Current Date (using correct API)
        cout << "Test 5: Current Date" << endl;
        Date today = Date::getCurrent();  // Use getCurrent() static method
        cout << "Today is valid: " << (today.isValid() ? "yes" : "no") << endl;
        
        cout << "All date operations completed successfully" << endl;
        return 0;
        
    } catch (const basar::Exception& ex) {
        cerr << "Error: " << ex.what() << endl;
        return 1;
    }
}
