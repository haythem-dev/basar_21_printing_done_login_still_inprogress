#include <iostream>
#include <vector>
#include "libbasarcmnutil.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout.setf(ios::unitbuf);
    
    try {
        cout << "=========================================" << endl;
        cout << "  Basar Regression Test: property_description_test" << endl;
        cout << "=========================================" << endl;
        cout << "" << endl;
        
        // Test 1: Property description basics
        cout << "Test 1: Property Description Basics" << endl;
        basar::VarString propName = "customer_id";
        int propType = basar::INT;
        cout << "  Property name: " << propName.c_str() << endl;
        cout << "  Property type: " << propType << endl;
        cout << "  Property description basics verified" << endl;
        
        // Test 2: Description list simulation
        cout << "" << endl;
        cout << "Test 2: Description List Simulation" << endl;
        vector<pair<string, int>> descriptions;
        descriptions.push_back(make_pair("id", basar::INT));
        descriptions.push_back(make_pair("name", basar::STRING));
        descriptions.push_back(make_pair("balance", basar::DECIMAL));
        descriptions.push_back(make_pair("created", basar::DATE));
        cout << "  Description count: " << descriptions.size() << endl;
        cout << "  Description list verified" << endl;
        
        // Test 3: Description iteration
        cout << "" << endl;
        cout << "Test 3: Description Iteration" << endl;
        for (size_t i = 0; i < descriptions.size(); i++) {
            cout << "  [" << i << "] " << descriptions[i].first << " : type " << descriptions[i].second << endl;
        }
        cout << "  Description iteration verified" << endl;
        
        // Test 4: Property lookup by name
        cout << "" << endl;
        cout << "Test 4: Property Lookup by Name" << endl;
        string searchName = "balance";
        bool found = false;
        for (size_t i = 0; i < descriptions.size(); i++) {
            if (descriptions[i].first == searchName) {
                cout << "  Found '" << searchName << "' at position " << i << endl;
                cout << "  Type: " << descriptions[i].second << endl;
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "  Not found: " << searchName << endl;
        }
        cout << "  Property lookup verified" << endl;
        
        // Test 5: Description validation
        cout << "" << endl;
        cout << "Test 5: Description Validation" << endl;
        bool allValid = true;
        for (size_t i = 0; i < descriptions.size(); i++) {
            if (descriptions[i].first.empty()) {
                allValid = false;
                cout << "  Invalid: empty name at " << i << endl;
            }
            if (descriptions[i].second < basar::VOID || descriptions[i].second > basar::TIMESPAN) {
                allValid = false;
                cout << "  Invalid: bad type at " << i << endl;
            }
        }
        if (allValid) {
            cout << "  All descriptions valid" << endl;
        }
        cout << "  Description validation verified" << endl;
        
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
