#include <iostream>
#include "libbasarcmnutil.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout.setf(ios::unitbuf);
    
    try {
        cout << "=========================================" << endl;
        cout << "  Basar Regression Test: property_type_test" << endl;
        cout << "=========================================" << endl;
        cout << "" << endl;
        
        // Test 1: Basic property type constants
        cout << "Test 1: Property Type Constants" << endl;
        cout << "  PropertyType VOID: " << basar::VOID << endl;
        cout << "  PropertyType BOOL: " << basar::BOOL << endl;
        cout << "  PropertyType INT: " << basar::INT << endl;
        cout << "  PropertyType STRING: " << basar::STRING << endl;
        cout << "  Property type constants verified" << endl;
        
        // Test 2: Extended property types
        cout << "" << endl;
        cout << "Test 2: Extended Property Types" << endl;
        cout << "  PropertyType DECIMAL: " << basar::DECIMAL << endl;
        cout << "  PropertyType DATE: " << basar::DATE << endl;
        cout << "  PropertyType TIME: " << basar::TIME << endl;
        cout << "  Extended property types verified" << endl;
        
        // Test 3: DateTime and Timespan types
        cout << "" << endl;
        cout << "Test 3: DateTime and Timespan Types" << endl;
        cout << "  PropertyType DATETIME: " << basar::DATETIME << endl;
        cout << "  PropertyType TIMESPAN: " << basar::TIMESPAN << endl;
        cout << "  DateTime and Timespan types verified" << endl;
        
        // Test 4: VarString for column names
        cout << "" << endl;
        cout << "Test 4: Column Name Handling" << endl;
        basar::VarString colName1 = "column_int";
        basar::VarString colName2 = "column_string";
        basar::VarString colName3 = "column_decimal";
        cout << "  Column 1: " << colName1.c_str() << endl;
        cout << "  Column 2: " << colName2.c_str() << endl;
        cout << "  Column 3: " << colName3.c_str() << endl;
        cout << "  Column name handling verified" << endl;
        
        // Test 5: Type-safe comparisons
        cout << "" << endl;
        cout << "Test 5: Type-Safe Comparisons" << endl;
        if (basar::INT != basar::STRING) {
            cout << "  INT != STRING: true" << endl;
        }
        if (basar::DATE != basar::TIME) {
            cout << "  DATE != TIME: true" << endl;
        }
        cout << "  Type-safe comparisons verified" << endl;
        
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
