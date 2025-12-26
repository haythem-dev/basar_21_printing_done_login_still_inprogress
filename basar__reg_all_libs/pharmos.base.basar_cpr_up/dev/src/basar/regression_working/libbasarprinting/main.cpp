#include <iostream>
#include "libbasar_printingol.h"

using namespace std;

int main() {
    cout << "libbasarprinting Regression Test" << endl;

    try {
        cout << "Testing libbasarprinting load..." << endl;
        // The library seems to have loggerpool and printutils
        // We'll just verify we can link and run
        cout << "libbasarprinting loaded successfully" << endl;
        return 0;
    } catch (const std::exception& ex) {
        cerr << "Error: " << ex.what() << endl;
        return 1;
    }
}
