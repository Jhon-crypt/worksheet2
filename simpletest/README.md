# SimpleTest Framework Documentation

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Usage Guide](#usage-guide)
- [API Reference](#api-reference)
- [Examples](#examples)
- [Best Practices](#best-practices)
- [Implementation Details](#implementation-details)

## Overview

SimpleTest is a lightweight, header-only C++ testing framework designed for simplicity and ease of use while providing robust testing capabilities. It offers a clean, macro-based interface for writing unit tests with detailed reporting and statistics collection.

### Key Benefits
- Header-only implementation (no linking required)
- Minimal dependencies (standard C++ library only)
- Intuitive API with expressive test declarations
- Detailed test reporting with file and line information
- Support for test grouping and organization
- Configurable verbosity levels

## Features

### 1. Test Organization
- Grouping tests into logical categories
- Hierarchical test structure
- Individual test case isolation
- Test suite management

### 2. Assertion System
- Equality comparisons
- Conditional checks
- Custom message support
- Detailed failure reporting

### 3. Test Execution
- Automatic test registration
- Sequential test execution
- Group-based execution
- Statistics collection

### 4. Reporting
- Verbose and summary modes
- Pass/fail statistics
- Detailed error messages
- Source location tracking

## Usage Guide

### Basic Test Structure
```cpp
#include <simpletest.h>

// Define a test in the "BumpAllocator" group
DEFINE_TEST_G(BasicAllocation, BumpAllocator) {
    // Test implementation
    int* x = allocator.alloc<int>();
    TEST_MESSAGE(x != nullptr, "Allocation should succeed");
    
    if (x != nullptr) {
        *x = 42;
        TEST_EQUAL(*x, 42, "Value should be stored correctly");
    }
}
```

### Test Groups
```cpp
// Group definition
struct TestGroup {
    const char* name;
    std::vector<std::function<void()>> tests;
};

// Group registration
TestGroup test_groups[] = {
    {
        "BumpAllocator",
        {
            TEST_BasicAllocation_BumpAllocator,
            TEST_AllocationFailure_BumpAllocator
        }
    }
};
```

### Running Tests
```cpp
int main() {
    bool pass = true;
    
    for (const auto& group : test_groups) {
        std::cout << "\nRunning test group: " << group.name << "\n";
        for (const auto& test : group.tests) {
            test();
        }
        pass &= TestFixture::ExecuteTestGroup(group.name, TestFixture::Verbose);
    }
    
    return pass ? 0 : 1;
}
```

## API Reference

### Test Definition Macros

#### DEFINE_TEST_G
```cpp
DEFINE_TEST_G(name, group)
```
Defines a new test case within a specified group.
- `name`: Name of the test case
- `group`: Group the test belongs to

### Assertion Macros

#### TEST_MESSAGE
```cpp
TEST_MESSAGE(condition, message)
```
Verifies a condition and reports the result.
- `condition`: Boolean expression to test
- `message`: Description of the test

#### TEST_EQUAL
```cpp
TEST_EQUAL(actual, expected, message)
```
Compares two values for equality.
- `actual`: Value being tested
- `expected`: Expected value
- `message`: Description of the comparison

### TestFixture Class

```cpp
class TestFixture {
public:
    static bool ExecuteTestGroup(const char* groupName, bool verbose);
    static void IncrementPassed();
    static void IncrementTotal();
    static bool Verbose;
    
private:
    static int passedTests;
    static int totalTests;
};
```

## Examples

### 1. Basic Test Case
```cpp
DEFINE_TEST_G(SimpleTest, BasicFeatures) {
    int x = 42;
    TEST_EQUAL(x, 42, "Integer should equal 42");
    TEST_MESSAGE(x > 0, "Value should be positive");
}
```

### 2. Complex Test Case
```cpp
DEFINE_TEST_G(ComplexTest, DataStructures) {
    std::vector<int> vec{1, 2, 3};
    TEST_EQUAL(vec.size(), 3, "Vector size should be 3");
    TEST_MESSAGE(!vec.empty(), "Vector should not be empty");
    TEST_EQUAL(vec[0], 1, "First element should be 1");
}
```

### 3. Test Group Organization
```cpp
TestGroup test_groups[] = {
    {
        "BasicFeatures",
        {
            TEST_SimpleTest_BasicFeatures
        }
    },
    {
        "DataStructures",
        {
            TEST_ComplexTest_DataStructures
        }
    }
};
```

## Best Practices

### 1. Test Organization
- Group related tests together
- Use descriptive test and group names
- Keep tests focused and concise
- Follow the Arrange-Act-Assert pattern

### 2. Assertion Usage
- Use specific assertions when possible
- Provide meaningful error messages
- Include context in failure messages
- Test both positive and negative cases

### 3. Test Independence
- Avoid test interdependencies
- Clean up resources after tests
- Don't assume execution order
- Initialize required state in each test

### 4. Error Handling
- Test error conditions explicitly
- Verify exception cases
- Check boundary conditions
- Validate error messages

## Implementation Details

### 1. Test Registration
The framework uses static registration of test functions through macros, which:
- Creates uniquely named functions
- Registers tests with their groups
- Maintains test metadata

### 2. Test Execution
Tests are executed through the TestFixture class, which:
- Manages test state
- Tracks statistics
- Handles reporting
- Controls verbosity

### 3. Memory Management
The framework is designed to:
- Minimize dynamic allocation
- Clean up resources automatically
- Prevent memory leaks
- Handle test isolation

### 4. Error Reporting
The reporting system provides:
- File and line information
- Detailed error messages
- Pass/fail statistics
- Test execution summary

## Contributing

When adding new features or modifications:
1. Maintain backward compatibility
2. Add appropriate documentation
3. Include example usage
4. Follow existing code style
5. Add tests for new features

## Version History

### v1.0.0
- Initial release
- Basic test macros
- Group support
- Statistics tracking
- Verbose reporting 