#ifndef SIMPLETEST_H
#define SIMPLETEST_H

#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

// Test fixture class to manage test execution and reporting
class TestFixture {
private:
    static int passedTests;
    static int totalTests;

public:
    static bool ExecuteTestGroup(const char* groupName, bool verbose = false) {
        std::cout << "\nTest Group: " << groupName << std::endl;
        std::cout << "Passed " << passedTests << " out of " << totalTests << " tests." << std::endl;
        bool allPassed = (passedTests == totalTests);
        // Reset counters for next group
        passedTests = 0;
        totalTests = 0;
        return allPassed;
    }

    static void IncrementPassed() { passedTests++; }
    static void IncrementTotal() { totalTests++; }
    static bool Verbose;
};

bool TestFixture::Verbose = true;  // Set to true by default for better visibility
int TestFixture::passedTests = 0;
int TestFixture::totalTests = 0;

// Macro to define a test function
#define DEFINE_TEST_G(name, group) void TEST_##name##_##group()

// Test assertion macros
#define TEST_MESSAGE(condition, message) \
    do { \
        TestFixture::IncrementTotal(); \
        if (!(condition)) { \
            std::cout << "FAILED: " << message << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
        } else { \
            TestFixture::IncrementPassed(); \
            if (TestFixture::Verbose) { \
                std::cout << "PASSED: " << message << std::endl; \
            } \
        } \
    } while(0)

#define TEST_EQUAL(actual, expected, message) \
    do { \
        TestFixture::IncrementTotal(); \
        if (!((actual) == (expected))) { \
            std::cout << "FAILED: " << message << "\n" \
                     << "Expected: " << (expected) << "\n" \
                     << "Actual: " << (actual) << "\n" \
                     << "[" << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
        } else { \
            TestFixture::IncrementPassed(); \
            if (TestFixture::Verbose) { \
                std::cout << "PASSED: " << message << std::endl; \
            } \
        } \
    } while(0)

#endif // SIMPLETEST_H 