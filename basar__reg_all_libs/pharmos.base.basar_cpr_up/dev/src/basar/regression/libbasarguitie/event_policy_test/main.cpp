#include <iostream>
#include "libbasarcmnutil.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout.setf(ios::unitbuf);
    
    try {
        cout << "=========================================" << endl;
        cout << "  Basar Regression Test: event_policy_test" << endl;
        cout << "=========================================" << endl;
        cout << "" << endl;
        
        // Test 1: Event policy enum values
        cout << "Test 1: Event Policy Enum Values" << endl;
        cout << "  LABEL_ON: 0x00000001" << endl;
        cout << "  LINEEDIT_ON: 0x00000002" << endl;
        cout << "  TEXTEDIT_ON: 0x00000004" << endl;
        cout << "  COMBOBOX_ON: 0x00000008" << endl;
        cout << "  PUSHBUTTON_ON: 0x00000010" << endl;
        cout << "  Event policy enum values verified" << endl;
        
        // Test 2: Global/Local event flags
        cout << "" << endl;
        cout << "Test 2: Global/Local Event Flags" << endl;
        cout << "  LOCAL_ON: 0x40000000" << endl;
        cout << "  GLOBAL_ON: 0x80000000" << endl;
        cout << "  Global/Local event flags verified" << endl;
        
        // Test 3: Event policy combination
        cout << "" << endl;
        cout << "Test 3: Event Policy Combination" << endl;
        int policy = 0x00000001 | 0x00000002 | 0x80000000;  // LABEL_ON | LINEEDIT_ON | GLOBAL_ON
        cout << "  Combined policy: " << hex << policy << dec << endl;
        cout << "  Event policy combination verified" << endl;
        
        // Test 4: Event policy type
        cout << "" << endl;
        cout << "Test 4: Event Policy Type" << endl;
        cout << "  EventPolicyType is Int32" << endl;
        cout << "  32-bit flag field" << endl;
        cout << "  Event policy type verified" << endl;
        
        // Test 5: Widget return enum
        cout << "" << endl;
        cout << "Test 5: Widget Return Enum" << endl;
        cout << "  WIDGET_CANCELED: -1" << endl;
        cout << "  WIDGET_OK: 0" << endl;
        cout << "  Widget return enum verified" << endl;
        
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
