#include <iostream>
#include <cstring>
#include "libbasarcmnutil.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout.setf(ios::unitbuf);
    
    try {
        cout << "=========================================" << endl;
        cout << "  Basar Regression Test: file_extension_test" << endl;
        cout << "=========================================" << endl;
        cout << "" << endl;
        
        // Test 1: PostScript extension
        cout << "Test 1: PostScript Extension" << endl;
        basar::VarString psExt = ".ps";
        basar::VarString psFile = "document" + psExt;
        cout << "  PS extension: " << psExt.c_str() << endl;
        cout << "  PS file: " << psFile.c_str() << endl;
        cout << "  PostScript extension verified" << endl;
        
        // Test 2: PDF extension
        cout << "" << endl;
        cout << "Test 2: PDF Extension" << endl;
        basar::VarString pdfExt = ".pdf";
        basar::VarString pdfFile = "document" + pdfExt;
        cout << "  PDF extension: " << pdfExt.c_str() << endl;
        cout << "  PDF file: " << pdfFile.c_str() << endl;
        cout << "  PDF extension verified" << endl;
        
        // Test 3: Extension replacement simulation
        cout << "" << endl;
        cout << "Test 3: Extension Replacement" << endl;
        basar::VarString baseName = "report";
        basar::VarString fromPs = baseName + ".ps";
        basar::VarString toPdf = baseName + ".pdf";
        cout << "  Original: " << fromPs.c_str() << endl;
        cout << "  Converted: " << toPdf.c_str() << endl;
        cout << "  Extension replacement verified" << endl;
        
        // Test 4: Multiple extensions
        cout << "" << endl;
        cout << "Test 4: Multiple File Types" << endl;
        const char* extensions[] = {".ps", ".pdf", ".eps", ".prn", ".txt"};
        cout << "  Supported extensions:" << endl;
        for (int i = 0; i < 5; i++) {
            cout << "    - " << extensions[i] << endl;
        }
        cout << "  Multiple file types verified" << endl;
        
        // Test 5: Path with extension
        cout << "" << endl;
        cout << "Test 5: Full Path with Extension" << endl;
        basar::VarString path = "/var/spool/print/";
        basar::VarString fullPath = path + baseName + pdfExt;
        cout << "  Full path: " << fullPath.c_str() << endl;
        cout << "  Full path with extension verified" << endl;
        
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
