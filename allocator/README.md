# Memory Allocator

This directory contains the implementation of a high-performance memory allocation system, primarily focused on a bump allocator design. The allocator is designed to work efficiently with the fiber scheduler system, providing fast and predictable memory allocation for fiber stacks and other fiber-related data structures.

## Table of Contents

1. [Overview](#overview)
2. [Bump Allocator](#bump-allocator)
3. [Architecture](#architecture)
4. [Implementation Details](#implementation-details)
5. [Memory Management](#memory-management)
6. [Performance Optimization](#performance-optimization)
7. [Integration Guide](#integration-guide)
8. [Testing](#testing)
9. [Advanced Topics](#advanced-topics)
10. [Troubleshooting](#troubleshooting)
11. [Future Improvements](#future-improvements)
12. [Contributing](#contributing)
13. [References](#references)

## Overview

The memory allocator system is a critical component of the fiber scheduler, providing efficient and predictable memory management. It is designed with the following goals:

- **High Performance**: Optimized for fiber operations
- **Predictability**: Consistent allocation times
- **Safety**: Protected memory operations
- **Flexibility**: Configurable for different use cases
- **Efficiency**: Minimal memory overhead

### Key Features

- Fast O(1) allocations
- Zero fragmentation for stack allocations
- Cache-friendly memory layout
- Configurable memory protection
- Debug support and monitoring
- SIMD alignment support
- Custom allocation strategies

### Use Cases

1. **Fiber Stacks**
   - Dynamic stack allocation
   - Stack overflow protection
   - Efficient context switching

2. **Task Data**
   - Temporary allocations
   - Message buffers
   - Task context storage

3. **Shared Resources**
   - Pool allocations
   - Shared memory regions
   - Inter-fiber communication

## Bump Allocator

The bump allocator (`BumpAllocator`) is a highly efficient memory allocator that works by incrementing a pointer through a pre-allocated memory region.

### Core Principles

1. **Sequential Allocation**
   ```cpp
   class BumpAllocator {
       void* start_;      // Start of memory region
       void* current_;    // Current allocation position
       void* end_;        // End of memory region
       size_t size_;      // Total size of region
   };
   ```

2. **Memory Layout**
   ```
   Memory Region Layout:
   +----------------+----------------------+------------------+
   | Header (64B)   | Allocation Space    | Footer (64B)    |
   +----------------+----------------------+------------------+
   
   Guard Page Configuration:
   +------------+----------------+----------------+------------+
   | Guard Page | Header Region  | Usable Memory | Guard Page |
   | (4KB)      | (Cache Line)   | (Configurable)| (4KB)     |
   +------------+----------------+----------------+------------+
   ```

3. **Alignment Requirements**
   ```cpp
   // Alignment calculations
   static constexpr size_t kDefaultAlignment = 16;
   static constexpr size_t kCacheLineSize = 64;
   static constexpr size_t kPageSize = 4096;
   ```

### Core Features

1. **Fast Allocation**
   - Single pointer increment for allocations
   - No complex metadata management
   - Aligned memory access support
   - Zero-overhead for common allocation sizes
   - SIMD-friendly memory alignment
   - Cache line optimization

2. **Memory Management**
   - Pre-allocated memory regions
   - Configurable block sizes
   - Support for aligned allocations
   - Memory boundary checks
   - Memory mapping control
   - Custom allocation strategies

3. **Safety Features**
   - Guard pages for stack overflow protection
   - Memory boundary validation
   - Alignment enforcement
   - Debug mode with additional checks
   - Memory access validation
   - Corruption detection

4. **Performance Features**
   - Cache-friendly allocations
   - SIMD alignment support
   - Branch prediction optimization
   - Memory prefetching
   - Minimal fragmentation
   - Fast reset operations

### Usage Examples

```cpp
// Basic usage
BumpAllocator allocator(1024 * 1024); // 1MB block
void* ptr = allocator.allocate(1024); // Allocate 1KB

// Aligned allocation
void* aligned_ptr = allocator.allocate_aligned(1024, 16); // 16-byte aligned

// SIMD-aligned allocation
void* simd_ptr = allocator.allocate_simd(1024); // AVX-512 aligned

// Stack allocation with guard pages
StackAllocation stack = allocator.allocate_stack(64 * 1024); // 64KB stack

// Reset the allocator
allocator.reset();

// Advanced usage with custom alignment
struct CustomAlignmentConfig {
    static constexpr size_t alignment = 32;
    static constexpr bool use_guard_pages = true;
    static constexpr size_t min_size = 64;
};

BumpAllocator<CustomAlignmentConfig> custom_allocator(2 * 1024 * 1024);
```

## Architecture

### Component Overview

```
+------------------------+
|    Fiber Scheduler    |
+------------------------+
           ↓
+------------------------+
|   Memory Allocator    |
+------------------------+
           ↓
+------------------------+
|    Memory Manager     |
+------------------------+
           ↓
+------------------------+
|   Physical Memory     |
+------------------------+
```

### Memory Hierarchy

1. **Top Level**
   - Fiber scheduler interface
   - Allocation strategy selection
   - Memory policy enforcement

2. **Middle Level**
   - Bump allocator implementation
   - Memory pool management
   - Cache optimization

3. **Low Level**
   - Physical memory management
   - Page table manipulation
   - Hardware interaction

### Implementation Details

1. **Memory Layout**
   ```
   Virtual Memory Layout:
   +------------------+--------------------+------------------+
   | Low Guard Region | Allocation Region  | High Guard Region|
   | (Protected)      | (R/W Access)      | (Protected)      |
   +------------------+--------------------+------------------+
   
   Allocation Block:
   +----------+---------------+------------------+----------+
   | Metadata | Padding       | User Data        | Padding |
   | (16B)    | (Alignment)   | (Requested Size) | (Align) |
   +----------+---------------+------------------+----------+
   ```

2. **Allocation Strategy**
   ```cpp
   void* BumpAllocator::allocate(size_t size) {
       // Alignment calculation
       size_t aligned_size = align_up(size, kDefaultAlignment);
       
       // Boundary check
       if (current_ + aligned_size > end_) {
           throw std::bad_alloc();
       }
       
       // Allocation
       void* result = current_;
       current_ = static_cast<char*>(current_) + aligned_size;
       
       return result;
   }
   ```

3. **Performance Characteristics**
   - Allocation: O(1)
   - Reset: O(1)
   - Memory Overhead: Minimal (alignment padding only)
   - Cache Performance: Excellent (sequential access)

### Memory Management

1. **Resource Management**
   ```cpp
   class MemoryManager {
       // Memory pool configuration
       struct PoolConfig {
           size_t block_size;
           size_t block_count;
           bool use_guard_pages;
       };
       
       // Memory pool implementation
       class MemoryPool {
           BumpAllocator allocator_;
           std::vector<void*> free_blocks_;
       };
   };
   ```

2. **Protection Mechanisms**
   - Guard page installation
   - Memory access validation
   - Corruption detection
   - Overflow protection

3. **Debug Support**
   ```cpp
   struct DebugConfig {
       bool track_allocations;
       bool validate_access;
       bool check_corruption;
       bool log_operations;
   };
   ```

## Performance Optimization

### Cache Optimization

1. **Alignment Strategies**
   ```cpp
   // Cache line alignment
   static constexpr size_t kCacheLineSize = 64;
   
   template<typename T>
   T* align_to_cache_line(void* ptr) {
       uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
       addr = (addr + kCacheLineSize - 1) & ~(kCacheLineSize - 1);
       return reinterpret_cast<T*>(addr);
   }
   ```

2. **SIMD Support**
   ```cpp
   // SIMD alignment requirements
   static constexpr size_t kSSEAlignment = 16;
   static constexpr size_t kAVXAlignment = 32;
   static constexpr size_t kAVX512Alignment = 64;
   ```

### Memory Access Patterns

1. **Sequential Access**
   - Forward allocation
   - Bulk operations
   - Prefetching hints

2. **Pooling Strategies**
   - Size-based pools
   - Usage-based pools
   - Hybrid approaches

## Integration Guide

### Fiber Integration

1. **Stack Management**
   ```cpp
   struct FiberStack {
       void* stack_ptr;
       size_t stack_size;
       void* guard_low;
       void* guard_high;
   };
   
   FiberStack allocate_fiber_stack(size_t size) {
       // Implementation
   }
   ```

2. **Context Switching**
   ```cpp
   struct FiberContext {
       void* stack_pointer;
       void* instruction_pointer;
       // Additional context data
   };
   ```

### Resource Management

1. **Automatic Cleanup**
   ```cpp
   class ScopedAllocation {
       BumpAllocator& allocator_;
       void* ptr_;
       
   public:
       ScopedAllocation(BumpAllocator& alloc, size_t size);
       ~ScopedAllocation();
   };
   ```

2. **Memory Tracking**
   ```cpp
   struct AllocationStats {
       size_t total_allocated;
       size_t peak_usage;
       size_t allocation_count;
       // Additional metrics
   };
   ```

## Testing

### Test Categories

1. **Functional Tests**
   ```cpp
   void test_basic_allocation() {
       BumpAllocator allocator(1024);
       void* ptr = allocator.allocate(256);
       ASSERT(ptr != nullptr);
   }
   ```

2. **Performance Tests**
   ```cpp
   void benchmark_allocations() {
       const size_t iterations = 1000000;
       BumpAllocator allocator(1024 * 1024);
       
       auto start = std::chrono::high_resolution_clock::now();
       for (size_t i = 0; i < iterations; ++i) {
           void* ptr = allocator.allocate(16);
       }
       auto end = std::chrono::high_resolution_clock::now();
   }
   ```

3. **Stress Tests**
   ```cpp
   void stress_test_concurrent() {
       const size_t thread_count = 8;
       std::vector<std::thread> threads;
       
       for (size_t i = 0; i < thread_count; ++i) {
           threads.emplace_back([&]() {
               // Allocation stress test
           });
       }
   }
   ```

### Test Coverage

- Basic allocation/deallocation
- Alignment requirements
- Boundary conditions
- Memory safety
- Performance characteristics
- Concurrent access
- Error handling
- Memory leaks
- Fragmentation

## Advanced Topics

### Custom Allocator Policies

```cpp
template<typename Policy>
class CustomBumpAllocator {
    static constexpr size_t alignment = Policy::alignment;
    static constexpr bool use_guard_pages = Policy::use_guard_pages;
    
    // Implementation
};

struct HighPerformancePolicy {
    static constexpr size_t alignment = 64;
    static constexpr bool use_guard_pages = false;
};

struct SafetyPolicy {
    static constexpr size_t alignment = 16;
    static constexpr bool use_guard_pages = true;
};
```

### Memory Monitoring

```cpp
class MemoryMonitor {
public:
    struct Statistics {
        size_t total_allocated;
        size_t peak_usage;
        size_t allocation_count;
        double fragmentation_ratio;
    };
    
    void record_allocation(size_t size);
    void record_deallocation(size_t size);
    Statistics get_statistics() const;
};
```

## Troubleshooting

### Common Issues

1. **Memory Exhaustion**
   ```cpp
   try {
       void* ptr = allocator.allocate(size);
   } catch (const std::bad_alloc& e) {
       // Handle allocation failure
   }
   ```

2. **Alignment Issues**
   ```cpp
   // Check alignment
   bool is_properly_aligned(void* ptr, size_t alignment) {
       return reinterpret_cast<uintptr_t>(ptr) % alignment == 0;
   }
   ```

3. **Performance Problems**
   - Cache misalignment
   - False sharing
   - Memory fragmentation

### Debug Tools

1. **Memory Validation**
   ```cpp
   void validate_memory_region(void* start, size_t size) {
       // Validation implementation
   }
   ```

2. **Leak Detection**
   ```cpp
   class LeakDetector {
       std::unordered_map<void*, AllocationInfo> allocations_;
   public:
       void record_allocation(void* ptr, size_t size);
       void record_deallocation(void* ptr);
       void report_leaks() const;
   };
   ```

## Future Improvements

1. **Enhanced Features**
   - Thread-safe allocation support
   - Custom memory mapping options
   - Advanced debugging capabilities
   - Memory usage statistics
   - Allocation tracking
   - Performance profiling

2. **Performance Optimizations**
   - SIMD-aligned allocations
   - Cache line optimization
   - Memory prefetching
   - Custom alignment strategies
   - Branch prediction hints
   - Hardware-specific optimizations

3. **Integration Enhancements**
   - Additional allocator types
   - Pluggable allocator interface
   - Memory reclamation strategies
   - Advanced memory pooling
   - Cross-platform support
   - Container integration

4. **Monitoring and Debug**
   - Real-time statistics
   - Memory visualization
   - Performance analytics
   - Debug integration
   - Error tracking
   - Usage patterns

## Contributing

When contributing to the allocator implementation:

1. Follow the existing code style
2. Add tests for new functionality
3. Document performance implications
4. Consider backward compatibility
5. Update this documentation
6. Provide benchmark results
7. Consider platform specifics
8. Review security implications

### Code Style

```cpp
// Example of preferred code style
class ExampleClass {
public:
    explicit ExampleClass(size_t size);
    
    void* allocate(size_t size);
    void deallocate(void* ptr);
    
private:
    size_t size_;
    void* data_;
};
```

### Documentation Guidelines

1. **API Documentation**
   ```cpp
   /**
    * @brief Allocates memory with specified alignment
    * @param size Size of allocation in bytes
    * @param alignment Required alignment (power of 2)
    * @return Pointer to allocated memory
    * @throws std::bad_alloc on failure
    */
   void* allocate_aligned(size_t size, size_t alignment);
   ```

2. **Performance Documentation**
   ```cpp
   /**
    * Performance characteristics:
    * - Time complexity: O(1)
    * - Space overhead: alignment padding only
    * - Cache behavior: sequential access
    * - Thread safety: none
    */
   ```

## References

### Standards and Specifications

- [C++ Memory Model](https://en.cppreference.com/w/cpp/language/memory_model)
- [Memory Alignment](https://en.wikipedia.org/wiki/Data_structure_alignment)
- [Stack Overflow Protection](https://en.wikipedia.org/wiki/Stack_buffer_overflow)
- [Memory Management in C++](https://en.cppreference.com/w/cpp/memory)

### Additional Resources

1. **Research Papers**
   - "Fast and Space-Efficient Memory Allocators"
   - "Memory Allocation Strategies in C++"
   - "Efficient Implementation of the C++ Memory Model"

2. **Related Documentation**
   - CPU Architecture Manuals
   - Operating System Memory Management
   - Compiler Optimization Guides

3. **Tools and Libraries**
   - Memory Profilers
   - Debug Tools
   - Performance Analyzers 