# Tests

This directory contains the test suite for the fiber scheduler and memory management system. The tests are implemented using the SimpleTest framework, a lightweight, header-only testing framework designed for simplicity and effectiveness.

## Table of Contents

1. [Overview](#overview)
2. [Test Organization](#test-organization)
3. [Running Tests](#running-tests)
4. [Writing Tests](#writing-tests)
5. [Test Categories](#test-categories)
6. [Testing Framework](#testing-framework)
7. [Best Practices](#best-practices)
8. [Troubleshooting](#troubleshooting)

## Overview

The test suite provides comprehensive coverage of:
- Fiber context management
- Memory allocation and deallocation
- Scheduler functionality
- Synchronization primitives
- Error handling and edge cases

### Key Features
- Automated test execution
- Detailed failure reporting
- Test grouping and organization
- Performance benchmarking
- Memory leak detection

## Test Organization

Tests are organized into logical groups:

### 1. Fiber Tests
- Context creation and destruction
- Stack management
- Context switching
- Exception handling

### 2. Allocator Tests
- Basic allocation/deallocation
- Memory pool management
- Alignment handling
- Out-of-memory scenarios

### 3. Scheduler Tests
- Priority scheduling
- Work stealing
- Load balancing
- Fiber synchronization

### 4. Integration Tests
- Component interaction
- System-wide behavior
- Resource management
- Error propagation

## Running Tests

### Basic Usage
```bash
# Build and run all tests
cmake --build . --target test

# Run specific test groups
./test_bump_allocator
./test_context
./test_suite
```

### Configuration Options
```bash
# Run with verbose output
SIMPLETEST_VERBOSE=1 ./test_suite

# Run with memory checking
valgrind --leak-check=full ./test_suite

# Run with specific test filter
./test_suite --filter="BumpAllocator.*"
```

## Writing Tests

### Basic Test Structure
```cpp
#include "simpletest.h"

// Define a test in the "BumpAllocator" group
DEFINE_TEST_G(BasicAllocation, BumpAllocator) {
    BumpAllocator allocator(1024);  // Setup
    
    // Test case
    void* ptr = allocator.allocate(64);
    TEST_MESSAGE(ptr != nullptr, "Allocation should succeed");
    
    // Verification
    TEST_EQUAL(allocator.get_allocated_size(), 64,
               "Allocated size should match request");
}
```

### Test Fixtures
```cpp
struct AllocatorFixture {
    BumpAllocator allocator;
    static constexpr size_t POOL_SIZE = 1024;
    
    AllocatorFixture() : allocator(POOL_SIZE) {}
    
    void* allocate_test_block() {
        return allocator.allocate(64);
    }
};

DEFINE_TEST_G(FixtureTest, BumpAllocator) {
    AllocatorFixture fixture;
    void* ptr = fixture.allocate_test_block();
    TEST_MESSAGE(ptr != nullptr, "Fixture allocation should succeed");
}
```

### Error Testing
```cpp
DEFINE_TEST_G(ErrorHandling, BumpAllocator) {
    BumpAllocator allocator(64);
    
    // Test out-of-memory condition
    try {
        void* ptr = allocator.allocate(128);
        TEST_MESSAGE(false, "Should throw std::bad_alloc");
    } catch (const std::bad_alloc&) {
        TEST_MESSAGE(true, "Correctly threw bad_alloc");
    }
}
```

## Test Categories

### 1. Unit Tests
- Individual component testing
- Isolated functionality verification
- Edge case handling
- Error condition testing

### 2. Integration Tests
- Component interaction
- System behavior
- Resource management
- Error propagation

### 3. Performance Tests
- Allocation speed
- Context switching overhead
- Memory usage patterns
- Scalability verification

### 4. Stress Tests
- High load scenarios
- Resource exhaustion
- Concurrent operations
- Long-running stability

## Testing Framework

The SimpleTest framework provides:

### 1. Test Macros
```cpp
// Basic test definition
DEFINE_TEST_G(name, group)

// Assertions
TEST_MESSAGE(condition, message)
TEST_EQUAL(actual, expected, message)
```

### 2. Test Execution
```cpp
// Group execution
TestFixture::ExecuteTestGroup("BumpAllocator");

// Test statistics
TestFixture::IncrementPassed();
TestFixture::IncrementTotal();
```

### 3. Reporting Features
- Detailed failure messages
- Source location tracking
- Pass/fail statistics
- Test execution summary

## Best Practices

### 1. Test Organization
- Group related tests together
- Use descriptive test names
- Keep tests focused and concise
- Follow the Arrange-Act-Assert pattern

### 2. Resource Management
```cpp
DEFINE_TEST_G(ResourceManagement, BumpAllocator) {
    // Use RAII for resource cleanup
    struct Cleanup {
        BumpAllocator& alloc;
        void* ptr;
        ~Cleanup() { alloc.deallocate(ptr); }
    };
    
    BumpAllocator allocator(1024);
    void* ptr = allocator.allocate(64);
    Cleanup cleanup{allocator, ptr};
    
    // Test implementation...
}
```

### 3. Error Handling
- Test both success and failure paths
- Verify exception messages
- Check error conditions
- Validate error recovery

### 4. Performance Testing
```cpp
DEFINE_TEST_G(Performance, BumpAllocator) {
    BumpAllocator allocator(1024 * 1024);
    auto start = std::chrono::high_resolution_clock::now();
    
    // Perform operations
    for (int i = 0; i < 1000; i++) {
        void* ptr = allocator.allocate(64);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end - start);
    
    TEST_MESSAGE(duration.count() < 1000,
                "Allocations should complete within 1ms");
}
```

## Troubleshooting

### Common Issues

1. **Test Failures**
   - Check test prerequisites
   - Verify test environment
   - Review failure messages
   - Check resource cleanup

2. **Memory Leaks**
   - Use valgrind for detection
   - Implement cleanup handlers
   - Check resource management
   - Verify destructor behavior

3. **Performance Issues**
   - Profile test execution
   - Check system resources
   - Verify test isolation
   - Monitor memory usage

### Debug Tools
```cpp
// Debug logging helper
struct TestLogger {
    static void log_allocation(size_t size, void* ptr) {
        std::cout << "Allocated " << size << " bytes at " << ptr << "\n";
    }
    
    static void log_deallocation(void* ptr) {
        std::cout << "Deallocated memory at " << ptr << "\n";
    }
};
```

## Contributing

When adding new tests:
1. Follow existing test patterns
2. Add appropriate documentation
3. Include performance considerations
4. Verify resource cleanup
5. Add test coverage

## See Also

- [SimpleTest Framework Documentation](../simpletest/README.md)
- [Fiber Implementation Tests](../fibers/README.md)
- [Allocator Tests](../allocator/README.md)
- [Main Project README](../README.md) 