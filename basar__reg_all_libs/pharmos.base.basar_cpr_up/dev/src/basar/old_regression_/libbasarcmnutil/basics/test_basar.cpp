#include <iostream>
#include "libbasarcmnutil.h"
#include <UnitTest++/UnitTest++.h>

using namespace basar::cmnutil;
using namespace std;

// Basic string operations test
TEST(BasicStringTest) {
    BString str1("Hello");
    BString str2(" World");
    BString result = str1 + str2;

    CHECK_EQUAL("Hello World", result.c_str());
}

// Basic locale test
TEST(BasicLocaleTest) {
    Locale locale = Locale::getDefault();
    CHECK(locale.isValid());
}

// Basic I18nString test
TEST(BasicI18nStringTest) {
    I18nString str("test", Locale::getDefault());
    CHECK_EQUAL("test", str.getString().c_str());
}

// Basic logging test
TEST(BasicLoggingTest) {
    try {
        log4cplus::Logger logger = Logging::getInstance("test.basics");
        CHECK(logger.getName() == "test.basics");
    } catch (const basar::Exception& ex) {
        FAIL(ex.what());
    }
}

int main(int argc, char* argv[]) {
    cout << "Running basic libbasarcmnutil tests..." << endl;
    int result = UnitTest::RunAllTests();
    cout << "Tests completed with result: " << result << endl;
    return result;
}