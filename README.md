# Memory Management System Implementation

## Table of contents:

- [Task 1](#task-1)
  - [Overview](#task-1-overview)
  - [Main Implementation](#task-1-main-implementation)
  - [Output & Observations](#task-1-output--observations)
- [Task 2](#task-2)
  - [Overview](#task-2-overview)
  - [Implementation](#task-2-implementation)
  - [Output & Observations](#task-2-output--observations)
- [Task 3](#task-3)
  - [Overview](#task-3-overview)
  - [Implementation](#task-3-implementation)
  - [Output & Observations](#task-3-output--observations)

## Project Overview

This project implements a sophisticated memory management system with a focus on efficient allocation strategies. The core component is a bump allocator, which provides fast and efficient memory allocation through a linear allocation strategy. The project includes comprehensive testing and benchmarking frameworks to ensure reliability and measure performance.

Key features of the project include:
- Template-based bump allocator implementation
- Comprehensive test suite with custom testing framework
- Performance benchmarking and comparison with standard allocators
- Memory alignment and boundary management
- Allocation tracking and statistics

## Task 1

### Task 1 Overview

Task 1 implements a bump allocator, a fundamental memory allocation strategy that manages memory in a linear fashion. The bump allocator maintains a pointer to the next available memory location and "bumps" this pointer forward with each allocation. This approach provides extremely fast allocation times with minimal overhead.

### Task 1 Main Implementation

#### Data Structures

1. **BumpAllocator Class Template**
   ```cpp
   template<size_t N>
   class BumpAllocator {
   private:
       std::array<uint8_t, N> buffer;  // Fixed-size memory buffer
       size_t current_pos;             // Current allocation position
       size_t alloc_count;            // Number of active allocations
       // ... other implementation details
   };
   ```

2. **Memory Layout**
   - Fixed-size buffer using std::array for type safety
   - Alignment handling for different data types
   - Pointer arithmetic for memory management

3. **State Management**
   - Current position tracking
   - Allocation counter
   - Memory boundary checks
   - Alignment calculations

#### Key Functions

1. **Single Object Allocation**
   ```cpp
   template<typename T>
   T* alloc() {
       // Alignment calculation
       // Boundary checking
       // Memory allocation
       // Return typed pointer
   }
   ```

2. **Array Allocation**
   ```cpp
   template<typename T>
   T* alloc(size_t n) {
       // Size calculation
       // Memory availability check
       // Array allocation
       // Return array pointer
   }
   ```

3. **Deallocation**
   ```cpp
   void dealloc() {
       // Update allocation counter
       // Handle memory tracking
   }
   ```

4. **Memory Management**
   ```cpp
   size_t remaining_space() const;  // Available memory calculation
   size_t allocations() const;      // Active allocations tracking
   ```

#### Memory Alignment

The allocator handles memory alignment requirements:
- Calculates proper alignment for different types
- Ensures aligned memory access
- Prevents undefined behavior
- Optimizes memory access patterns

#### Error Handling

1. **Allocation Failures**
   - Out of memory conditions
   - Alignment requirements not met
   - Invalid size requests

2. **Safety Checks**
   - Buffer overflow prevention
   - Alignment validation
   - Size boundary verification

### Task 1 Output & Observations

#### Successful Operations

1. **Type Handling**
   - Proper allocation of fundamental types (int, double, char)
   - Correct handling of user-defined types
   - Array allocations with proper alignment

2. **Memory Management**
   - Accurate space tracking
   - Proper alignment maintenance
   - Efficient memory utilization

#### Performance Characteristics

1. **Allocation Speed**
   - Constant-time allocation operations
   - Minimal overhead per allocation
   - Fast pointer arithmetic

2. **Memory Efficiency**
   - Low fragmentation
   - Predictable memory usage
   - Efficient space utilization

## Task 2

### Task 2 Overview

Task 2 implements a comprehensive testing framework to verify the correctness and reliability of the bump allocator. The testing suite uses a custom framework that provides detailed feedback and statistics about test execution.

### Task 2 Implementation

#### Test Framework Architecture

1. **Test Fixture Class**
   ```cpp
   class TestFixture {
   public:
       static bool ExecuteTestGroup(const char* groupName, bool verbose);
       static void IncrementPassed();
       static void IncrementTotal();
       // ... other testing utilities
   };
   ```

2. **Test Macros**
   ```cpp
   #define TEST_MESSAGE(condition, message)
   #define TEST_EQUAL(actual, expected, message)
   #define DEFINE_TEST_G(name, group)
   ```

#### Test Categories

1. **Basic Allocation Tests**
   - Single object allocation
   - Array allocation
   - Different type allocations
   - Alignment verification

2. **Memory Management Tests**
   - Space calculation accuracy
   - Allocation counting
   - Boundary conditions
   - Deallocation behavior

3. **Error Handling Tests**
   - Out of memory conditions
   - Invalid size requests
   - Alignment requirements
   - Buffer overflow prevention

4. **Performance Tests**
   - Allocation speed
   - Memory utilization
   - Operation overhead

### Task 2 Output & Observations

#### Test Results

1. **Allocation Tests**
   - Successful allocation of various types
   - Proper memory alignment
   - Correct pointer arithmetic
   - Array handling verification

2. **Memory Management**
   - Accurate space tracking
   - Proper deallocation
   - Boundary checking
   - Alignment maintenance

3. **Error Handling**
   - Proper null returns on failure
   - Accurate error conditions
   - Memory protection

#### Statistical Analysis

1. **Test Coverage**
   - Number of tests executed
   - Pass/fail ratios
   - Test categories coverage

2. **Performance Metrics**
   - Allocation time measurements
   - Memory usage patterns
   - Operation overhead analysis

## Task 3

### Task 3 Overview

Task 3 focuses on performance benchmarking of the bump allocator, comparing it with standard allocation methods and analyzing its efficiency in various scenarios. The benchmarking suite provides detailed metrics and analysis of the allocator's behavior under different conditions.

### Task 3 Implementation

#### Benchmark Framework

1. **Measurement Infrastructure**
   ```cpp
   class BenchmarkSuite {
       // Timing measurements
       // Memory tracking
       // Statistical analysis
   };
   ```

2. **Test Scenarios**
   - Sequential allocations
   - Random access patterns
   - Mixed allocation sizes
   - Stress testing

#### Performance Metrics

1. **Time Measurements**
   - Allocation speed
   - Deallocation time
   - Operation overhead

2. **Memory Analysis**
   - Space utilization
   - Fragmentation patterns
   - Alignment overhead

3. **Comparison Metrics**
   - Standard allocator comparison
   - Memory usage patterns
   - Operation costs

#### Analysis Tools

1. **Statistical Analysis**
   - Performance distributions
   - Average case analysis
   - Worst case scenarios

2. **Visualization**
   - Memory usage graphs
   - Performance charts
   - Comparison plots

### Task 3 Output & Observations

#### Performance Results

1. **Speed Metrics**
   - Fast allocation times
   - Minimal operation overhead
   - Efficient memory access

2. **Memory Efficiency**
   - Low fragmentation
   - Optimal space utilization
   - Alignment costs

#### Comparative Analysis

1. **Standard Allocator Comparison**
   - Speed advantages
   - Memory efficiency
   - Use case analysis

2. **Trade-off Analysis**
   - Flexibility vs. speed
   - Memory usage vs. performance
   - Implementation complexity

## Building and Running

### Prerequisites
- C++17 compatible compiler
- CMake 3.10 or higher
- Make or equivalent build system

### Build Instructions
```bash
mkdir build
cd build
cmake ..
make
```

### Running Tests
```bash
# Run all tests
./build/task2

# Run specific test groups
./build/task2 [test_group_name]
```

### Running Benchmarks
```bash
# Run all benchmarks
./build/task3

# Run specific benchmark
./build/task3 [benchmark_name]
``` 