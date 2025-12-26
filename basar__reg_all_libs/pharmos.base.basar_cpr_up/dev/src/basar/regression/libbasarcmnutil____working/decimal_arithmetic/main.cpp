#include <iostream>
#include "libbasarcmnutil.h"

using namespace basar::cmnutil;
using namespace std;

int main() {
    cout << "Basar Decimal Arithmetic Test" << endl;

    try {
        // Test 1: Decimal Creation
        cout << "Test 1: Decimal Creation" << endl;
        Decimal dec1("123.45");
        cout << "Decimal from string '123.45': " << dec1.toString().c_str() << endl;

        // Test 2: Decimal Addition
        cout << "Test 2: Decimal Addition" << endl;
        Decimal dec2("67.89");
        Decimal sum = dec1 + dec2;
        cout << "123.45 + 67.89 = " << sum.toString().c_str() << endl;

        // Test 3: Decimal Subtraction
        cout << "Test 3: Decimal Subtraction" << endl;
        Decimal diff = dec1 - dec2;
        cout << "123.45 - 67.89 = " << diff.toString().c_str() << endl;

        // Test 4: Decimal Multiplication
        cout << "Test 4: Decimal Multiplication" << endl;
        Decimal prod = dec1 * dec2;
        cout << "123.45 * 67.89 = " << prod.toString().c_str() << endl;

        // Test 5: Decimal Comparison
        cout << "Test 5: Decimal Comparison" << endl;
        Decimal d100("100.00");
        Decimal d200("200.00");
        cout << "100.00 < 200.00: " << (d100 < d200 ? "true" : "false") << endl;
        cout << "200.00 > 100.00: " << (d200 > d100 ? "true" : "false") << endl;

        // Test 6: Decimal Precision
        cout << "Test 6: Decimal Precision" << endl;
        Decimal precise("0.123456789");
        cout << "Precise value: " << precise.toString().c_str() << endl;

        cout << "All decimal operations completed successfully" << endl;
        return 0;

    } catch (const basar::Exception& ex) {
        cerr << "Error: " << ex.what() << endl;
        return 1;
    }
}