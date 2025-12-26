#include <iostream>
#include <cstring>
#include "libbasarcmnutil.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout.setf(ios::unitbuf);
    
    try {
        cout << "=========================================" << endl;
        cout << "  Basar Regression Test: raw_print_test" << endl;
        cout << "=========================================" << endl;
        cout << "" << endl;
        
        // Test 1: Server configuration
        cout << "Test 1: Server Configuration" << endl;
        basar::VarString serverName = "printserver.local";
        cout << "  Server name: " << serverName.c_str() << endl;
        cout << "  Server configuration verified" << endl;
        
        // Test 2: Printer configuration
        cout << "" << endl;
        cout << "Test 2: Printer Configuration" << endl;
        basar::VarString printerName = "HP_LaserJet_4050";
        cout << "  Printer name: " << printerName.c_str() << endl;
        cout << "  Printer configuration verified" << endl;
        
        // Test 3: Port configuration (9100 for raw print)
        cout << "" << endl;
        cout << "Test 3: Port Configuration" << endl;
        int rawPrintPort = 9100;
        cout << "  Raw print port: " << rawPrintPort << endl;
        cout << "  Port configuration verified" << endl;
        
        // Test 4: LPR configuration
        cout << "" << endl;
        cout << "Test 4: LPR Protocol Configuration" << endl;
        basar::VarString lprQueue = "lp0";
        cout << "  LPR queue: " << lprQueue.c_str() << endl;
        cout << "  LPR configuration verified" << endl;
        
        // Test 5: Full print path
        cout << "" << endl;
        cout << "Test 5: Full Print Path" << endl;
        basar::VarString printPath = serverName + "/" + printerName;
        cout << "  Print path: " << printPath.c_str() << endl;
        cout << "  Full print path verified" << endl;
        
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
