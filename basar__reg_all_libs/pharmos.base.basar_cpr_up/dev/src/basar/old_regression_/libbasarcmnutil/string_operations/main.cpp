
#include <iostream>
#include "libbasarcmnutil.h"

using namespace basar::cmnutil;
using namespace std;

int main() {
    cout << "Basar String Operations Test" << endl;
    
    try {
        // Test 1: String Creation
        cout << "Test 1: String Creation" << endl;
        String str1("Hello Basar");
        cout << "Created string: " << str1.c_str() << endl;
        
        // Test 2: String Concatenation
        cout << "Test 2: String Concatenation" << endl;
        String str2(" Framework");
        String concat = str1 + str2;
        cout << "Concatenated: " << concat.c_str() << endl;
        
        // Test 3: String Length
        cout << "Test 3: String Length" << endl;
        cout << "Length of 'Hello Basar': " << str1.length() << endl;
        
        // Test 4: String Comparison
        cout << "Test 4: String Comparison" << endl;
        String strA("Alpha");
        String strB("Beta");
        cout << "Alpha == Beta: " << (strA == strB ? "true" : "false") << endl;
        cout << "Alpha < Beta: " << (strA < strB ? "true" : "false") << endl;
        
        // Test 5: String Substring
        cout << "Test 5: String Empty Check" << endl;
        String empty;
        String notEmpty("content");
        cout << "Empty string isEmpty: " << (empty.length() == 0 ? "true" : "false") << endl;
        cout << "Non-empty string isEmpty: " << (notEmpty.length() == 0 ? "true" : "false") << endl;
        
        cout << "All string operations completed successfully" << endl;
        return 0;
        
    } catch (const basar::Exception& ex) {
        cerr << "Error: " << ex.what() << endl;
        return 1;
    }
}
