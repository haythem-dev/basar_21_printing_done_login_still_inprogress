//----------------------------------------------------------------------------
/*! \file test_basar.cpp
 *  \brief Independent regression test application for BASAR framework on AIX
 *  \author AI Assistant
 *  \date 2025-11-20
 *  \version 1.0.0
 *
 *  This application tests core BASAR framework features and provides
 *  a summary of successful and failing tests.
 */
//----------------------------------------------------------------------------

// Include system headers first to avoid AIX/Boost sched_yield() conflict
// #define _H_SYS_SCHED

#include <stdio.h>
#include <string.h>
#include <iostream>

// Then include UnitTest++ and BASAR headers
#include <UnitTest++/UnitTest++.h>

#include "libbasarcmnutil.h"
#include "libbasarcmnutil_config.h"
#include "libbasardebug.h"

#include <UnitTest++/TestReporterStdoutEx.h>

using namespace basar;
using namespace basar::cmnutil;

//------------------------------------------------------------------------------
bool g_RUNSHORT = false;  // short test mode; avoids long running tests

//------------------------------------------------------------------------------
static void printHeader(basar::ConstString suite)
{
    VarString ver1 = getFullVersion();
    VarString ver2 = ver1.right("GmbH ");

    printf("===========================================\n");
    printf("BASAR Framework Regression Test for AIX\n");
    printf("===========================================\n");
    printf("test mode : running %s\n", g_RUNSHORT ? "short" : "long");
    printf("test suite: running %s\n", suite ? suite : "all");
    printf("\n");
    printf("ver : %s\n", getVersion());
    printf("full: %s\n", ver1.left(" version").c_str());
    printf("      %s\n", ver2.left(" built:").c_str());
#ifndef NDEBUG
    printf("      %s\n", ver2.right("dbg ").c_str());
#else
    printf("      %s\n", ver2.right("rel ").c_str());
#endif
    printf("===========================================\n\n");
}

//------------------------------------------------------------------------------
// Independent test suite for BASAR framework
//------------------------------------------------------------------------------
SUITE(BASAR_String_Tests)
{
    //--------------------------------------------------------------------------
    TEST(VarString_Basic_Operations)
    {
        VarString str1("Hello");
        VarString str2("World");

        CHECK_EQUAL("Hello", str1.c_str());
        CHECK_EQUAL("World", str2.c_str());

        str1 += " ";
        str1 += str2;

        CHECK_EQUAL("Hello World", str1.c_str());
    }

    //--------------------------------------------------------------------------
    TEST(VarString_Upper_Lower)
    {
        VarString str("Test String");

        str.upper();
        CHECK_EQUAL("TEST STRING", str.c_str());

        str.lower();
        CHECK_EQUAL("test string", str.c_str());
    }

    //--------------------------------------------------------------------------
    TEST(VarString_Find_Operations)
    {
        VarString str("The quick brown fox");

        CHECK(str.find("quick") != VarString::npos);
        CHECK(str.find("slow") == VarString::npos);

        VarString::size_type pos = str.find("brown");
        CHECK(pos == 10);
    }

    //--------------------------------------------------------------------------
    TEST(VarString_SubString)
    {
        VarString str("ABCDEFGH");

        VarString sub = str.substr(2, 3);
        CHECK_EQUAL("CDE", sub.c_str());
    }

    //--------------------------------------------------------------------------
    TEST(VarString_Left_Right)
    {
        VarString str("Copyright (C) 2025 Company GmbH version 1.0");

        VarString left = str.left(" version");
        CHECK(left.find("GmbH") != VarString::npos);

        VarString right = str.right("GmbH ");
        CHECK(right.find("version") != VarString::npos);
    }

    //--------------------------------------------------------------------------
    TEST(VarString_Format)
    {
        VarString str;
        str.format("Value: %d, Text: %s", 42, "Test");

        CHECK(str.find("42") != VarString::npos);
        CHECK(str.find("Test") != VarString::npos);
    }

    //--------------------------------------------------------------------------
    TEST(VarString_Empty)
    {
        VarString str1;
        CHECK(str1.empty());

        VarString str2("Not Empty");
        CHECK(!str2.empty());

        str2.clear();
        CHECK(str2.empty());
    }

    //--------------------------------------------------------------------------
    TEST(ConstString_Operations)
    {
        ConstString cs1 = "Constant String";
        ConstString cs2 = "Constant String";
        ConstString cs3 = "Different";

        CHECK_EQUAL(cs1, cs2);
        CHECK(0 == strcmp(cs1, cs2));
        CHECK(0 != strcmp(cs1, cs3));
    }
}

//------------------------------------------------------------------------------
SUITE(BASAR_Exception_Tests)
{
    //--------------------------------------------------------------------------
    TEST(Exception_Basic)
    {
        try
        {
            ExceptionInfoStruct info("Test Context", "Testing exception handling",
                __FILE__, __LINE__);
            throw BasarException(info, "TestException");
        }
        catch (const BasarException& e)
        {
            VarString what = e.what();
            CHECK(what.find("TestException") != VarString::npos);
            CHECK(what.find("Test Context") != VarString::npos);
        }
    }

    //--------------------------------------------------------------------------
    TEST(RuntimeException_Basic)
    {
        try
        {
            ExceptionInfoStruct info("Runtime Error Context",
                "Runtime error occurred");
            throw BasarRuntimeException(info);
        }
        catch (const BasarRuntimeException& e)
        {
            VarString what = e.what();
            CHECK(what.find("Runtime Error Context") != VarString::npos);
            CHECK(what.find("Runtime error occurred") != VarString::npos);
        }
    }

    //--------------------------------------------------------------------------
    TEST(Exception_With_StackTrace)
    {
        try
        {
            ExceptionInfoStruct info("StackTrace Test",
                "Testing stacktrace functionality");
            BasarException ex(info, "StackTraceException");
            ex.fillInStackTrace("BASAR_Exception_Tests::Exception_With_StackTrace",
                __FILE__, __LINE__);
            throw ex;
        }
        catch (const BasarException& e)
        {
            VarString trace = e.stackTrace();
            CHECK(trace.find("stacktrace") != VarString::npos);
            std::cout << "Stack Trace:\n" << trace.c_str() << std::endl;
        }
    }
}

//------------------------------------------------------------------------------
SUITE(BASAR_Utility_Tests)
{
    //--------------------------------------------------------------------------
    TEST(GetExecDir)
    {
        VarString execDir = getExecDir();

        CHECK(!execDir.empty());
        CHECK(execDir.length() > 0);

        // Should contain a valid path
        std::cout << "Exec Dir: " << execDir.c_str() << std::endl;
    }

    //--------------------------------------------------------------------------
    TEST(GetLibDir)
    {
        VarString libDir = getLibDir();

        CHECK(!libDir.empty());
        CHECK(libDir.length() > 0);

        std::cout << "Lib Dir: " << libDir.c_str() << std::endl;
    }

    //--------------------------------------------------------------------------
    TEST(GetExecPath)
    {
        VarString execPath = getExecPath();

        CHECK(!execPath.empty());
        CHECK(execPath.length() > 0);

        // Should end with executable name
        std::cout << "Exec Path: " << execPath.c_str() << std::endl;
    }

    //--------------------------------------------------------------------------
    TEST(Version_Information)
    {
        VarString version = getVersion();
        CHECK(!version.empty());

        VarString fullVersion = getFullVersion();
        CHECK(!fullVersion.empty());
        CHECK(fullVersion.length() > version.length());

        std::cout << "Version: " << version.c_str() << std::endl;
        std::cout << "Full Version: " << fullVersion.c_str() << std::endl;
    }
}

//------------------------------------------------------------------------------
SUITE(BASAR_Config_Tests)
{
#if 0
    //--------------------------------------------------------------------------
    TEST(Config_Singleton)
    {
        // Config::getInstance() returns ConfigBase, not Config&
        ConfigBase& config1 = Config::getInstance();
        ConfigBase& config2 = Config::getInstance();

        // Should return same instance
        CHECK(&config1 == &config2);
    }

    //--------------------------------------------------------------------------
    TEST(Config_GetConfigFile)
    {
        VarString configPath;
        bool found = Config::getInstance().getConfigFile(configPath);

        if (found)
        {
            std::cout << "Config file found at: " << configPath.c_str() << std::endl;
            CHECK(!configPath.empty());
        }
        else
        {
            std::cout << "Config file not found (this is okay for testing)" << std::endl;
            CHECK(configPath.empty());
        }
    }

    //--------------------------------------------------------------------------
    TEST(Config_GetValue_NonExistent)
    {
        VarString value;
        bool found = Config::getInstance().getValue("nonexistent.key.test", value);

        CHECK_EQUAL(false, found);
        CHECK(value.empty());
    }

    //--------------------------------------------------------------------------
    TEST(Config_ToStream)
    {
        std::ostringstream out;
        Config::getInstance().toStream(out);

        VarString output = out.str().c_str();
        std::cout << "Config output length: " << output.length() << std::endl;

        // Output might be empty if no config file is found
        CHECK(true);
    }
#endif
}

//------------------------------------------------------------------------------
SUITE(BASAR_I18n_Tests)
{
    //--------------------------------------------------------------------------
    TEST(I18nString_Basic)
    {
        I18nString str("International");

        CHECK_EQUAL("International", str.c_str());

        str.upper();
        CHECK_EQUAL("INTERNATIONAL", str.c_str());
    }

    //--------------------------------------------------------------------------
    TEST(I18nString_Conversion)
    {
        I18nString i18n("Test");
        VarString var = i18n;

        CHECK_EQUAL("Test", var.c_str());
    }

    //--------------------------------------------------------------------------
    TEST(I18nString_Empty)
    {
        I18nString str1;
        CHECK(str1.empty());

        I18nString str2("Not Empty");
        CHECK(!str2.empty());
    }
}

//------------------------------------------------------------------------------
SUITE(BASAR_Memory_Tests)
{
    //--------------------------------------------------------------------------
    TEST(Debug_LeakCheck_Enabled)
    {
        // Leak check should be enabled by main()
        // This test just validates no crash occurs
        basar::debug::enableLeakCheck();
        CHECK(true);
    }
}

//------------------------------------------------------------------------------
SUITE(BASAR_Integration_Tests)
{
    //--------------------------------------------------------------------------
    TEST(String_And_Exception_Integration)
    {
        try
        {
            VarString filename("test_file.txt");
            filename.upper();

            if (filename.find("MISSING") != VarString::npos)
            {
                ExceptionInfoStruct info("File Operation", "File not found");
                throw BasarException(info, "FileError");
            }

            // Should not throw
            CHECK_EQUAL("TEST_FILE.TXT", filename.c_str());
        }
        catch (const BasarException&)
        {
            CHECK(false); // Should not reach here
        }
    }

    //--------------------------------------------------------------------------
    TEST(Multiple_String_Operations)
    {
        VarString path = getExecDir();
        path += "/";
        path += "test.dat";

        CHECK(path.find("test.dat") != VarString::npos);

        VarString pathUpper = path;
        pathUpper.upper();

        CHECK(pathUpper.find("TEST.DAT") != VarString::npos);
    }
#if 0
    //--------------------------------------------------------------------------
    TEST(Config_And_String_Integration)
    {
        VarString testKey("basar.test.key");
        testKey.lower();

        VarString value;
        Config::getInstance().getValue(testKey.c_str(), value);

        // Value might be empty, but operation should succeed
        CHECK(true);
    }
#endif
    //--------------------------------------------------------------------------
    TEST(Exception_Handling_Chain)
    {
        int exceptionsCaught = 0;

        try
        {
            try
            {
                ExceptionInfoStruct info("Inner Context", "Inner error");
                BasarException ex(info, "InnerException");
                ex.fillInStackTrace("Inner Level", __FILE__, __LINE__);
                throw ex;
            }
            catch (const BasarException& inner)
            {
                exceptionsCaught++;
                ExceptionInfoStruct info("Outer Context", "Outer error wrapping inner");
                BasarException outer(info, "OuterException");
                outer.fillInStackTrace("Outer Level", __FILE__, __LINE__);
                throw outer;
            }
        }
        catch (const BasarException& final)
        {
            exceptionsCaught++;
            VarString trace = final.stackTrace();
            CHECK(trace.find("Outer Level") != VarString::npos);
        }

        CHECK_EQUAL(2, exceptionsCaught);
    }
}

//------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    basar::debug::enableLeakCheck();

    // Try environment variable
    basar::ConstBuffer suite = getenv("BUTBASAR");

    if (1 < argc)
    {
        for (int i = 1; i < argc; ++i)
        {
            if (0 == strcmp("-short", argv[i]))
            {
                g_RUNSHORT = true;
                continue;
            }

            if (0 == strncmp("-suite=", argv[i], 7))
            {
                suite = argv[i] + 7;
                continue;
            }

            if (0 == strcmp("-help", argv[i]) || 0 == strcmp("-h", argv[i]))
            {
                printf("Usage: %s [options]\n", argv[0]);
                printf("\n");
                printf("BASAR Framework Regression Test for AIX\n");
                printf("\n");
                printf("Options:\n");
                printf("  -short         Run short test mode (skip long running tests)\n");
                printf("  -suite=NAME    Run specific test suite only\n");
                printf("  -h, -help      Show this help message\n");
                printf("\n");
                printf("Environment Variables:\n");
                printf("  BUTBASAR       Set test suite name\n");
                printf("\n");
                printf("Available Test Suites:\n");
                printf("  BASAR_String_Tests       - String manipulation tests\n");
                printf("  BASAR_Exception_Tests    - Exception handling tests\n");
                printf("  BASAR_Utility_Tests      - Utility function tests\n");
                printf("  BASAR_Config_Tests       - Configuration system tests\n");
                printf("  BASAR_I18n_Tests         - Internationalization tests\n");
                printf("  BASAR_Memory_Tests       - Memory management tests\n");
                printf("  BASAR_Integration_Tests  - Integration tests\n");
                printf("\n");
                return 0;
            }
        }
    }

    printHeader(suite);

    // Create custom reporter with detailed output
    UnitTest::TestReporterStdoutEx reporter(true);
    UnitTest::TestRunner runner(reporter);

    // Run tests and capture result
    int result = runner.RunTestsIf(UnitTest::Test::GetTestList(),
        suite,
        UnitTest::True(),
        0);

    // Print comprehensive summary
    printf("\n");
    printf("===========================================\n");
    printf("Test Execution Summary\n");
    printf("===========================================\n");

    if (result == 0)
    {
        printf("STATUS  : SUCCESS\n");
        printf("RESULT  : All tests passed\n");
        printf("FAILURES: 0\n");
    }
    else
    {
        printf("STATUS  : FAILURE\n");
        printf("RESULT  : %d test(s) failed\n", result);
        printf("FAILURES: %d\n", result);
    }

    printf("===========================================\n");
    printf("Test suite completed\n");
    printf("===========================================\n");

    return result;
}
