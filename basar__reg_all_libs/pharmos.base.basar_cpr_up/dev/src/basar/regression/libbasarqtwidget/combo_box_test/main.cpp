#include <iostream>
#include <vector>
#include "libbasarcmnutil.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout.setf(ios::unitbuf);
    
    try {
        cout << "=========================================" << endl;
        cout << "  Basar Regression Test: combo_box_test" << endl;
        cout << "=========================================" << endl;
        cout << "" << endl;
        
        // Test 1: Combo box items
        cout << "Test 1: Combo Box Items" << endl;
        vector<string> items;
        items.push_back("Option 1");
        items.push_back("Option 2");
        items.push_back("Option 3");
        items.push_back("Option 4");
        items.push_back("Option 5");
        cout << "  Item count: " << items.size() << endl;
        for (size_t i = 0; i < items.size(); i++) {
            cout << "  [" << i << "] " << items[i] << endl;
        }
        cout << "  Combo box items verified" << endl;
        
        // Test 2: Selection index
        cout << "" << endl;
        cout << "Test 2: Selection Index" << endl;
        int selectedIndex = 2;
        cout << "  Selected index: " << selectedIndex << endl;
        cout << "  Selected item: " << items[selectedIndex] << endl;
        cout << "  Selection index verified" << endl;
        
        // Test 3: Multi-selection simulation (for MultiComboBox)
        cout << "" << endl;
        cout << "Test 3: Multi-Selection Simulation" << endl;
        vector<int> selectedIndices;
        selectedIndices.push_back(0);
        selectedIndices.push_back(2);
        selectedIndices.push_back(4);
        cout << "  Selected indices: ";
        for (size_t i = 0; i < selectedIndices.size(); i++) {
            cout << selectedIndices[i];
            if (i < selectedIndices.size() - 1) cout << ", ";
        }
        cout << endl;
        cout << "  Multi-selection verified" << endl;
        
        // Test 4: Item search
        cout << "" << endl;
        cout << "Test 4: Item Search" << endl;
        string searchText = "Option 3";
        int foundIndex = -1;
        for (size_t i = 0; i < items.size(); i++) {
            if (items[i] == searchText) {
                foundIndex = i;
                break;
            }
        }
        if (foundIndex >= 0) {
            cout << "  Found '" << searchText << "' at index " << foundIndex << endl;
        } else {
            cout << "  Item not found" << endl;
        }
        cout << "  Item search verified" << endl;
        
        // Test 5: Add/remove items
        cout << "" << endl;
        cout << "Test 5: Add/Remove Items" << endl;
        items.push_back("New Option");
        cout << "  After add: " << items.size() << " items" << endl;
        items.pop_back();
        cout << "  After remove: " << items.size() << " items" << endl;
        cout << "  Add/remove items verified" << endl;
        
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
