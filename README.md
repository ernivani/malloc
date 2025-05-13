# Custom Malloc Implementation

A simple custom memory allocation system implemented in C.

## Overview

This project implements a basic heap memory allocator with the following features:

- Fixed-size heap (640KB)
- First-fit allocation strategy
- Chunk-based memory management
- Allocation and deallocation functions
- Memory chunk tracking

## Functions

- `void *heap_alloc(size_t size)` - Allocates memory of specified size
- `void heap_free(void *ptr)` - Frees previously allocated memory
- `void heap_collect(void)` - Garbage collection (not implemented yet)

## Building and Running

```bash
# Compile the project
gcc -Wall -Wextra -o heap main.c

# Run the executable
./heap
```

## Memory Management

The allocator maintains two chunk lists:
- `alloced_chunk`: Tracks allocated memory regions
- `freed_chunks`: Tracks available memory regions

When memory is allocated:
1. The allocator searches for a free chunk large enough
2. It splits the chunk if necessary
3. The allocated portion is moved to the allocated list
4. Any remainder stays in the free list

When memory is freed:
1. The chunk is moved from the allocated list to the free list
2. (Future improvement: Coalescing adjacent free chunks)

## Future Improvements

- Implement `heap_collect()` for garbage collection
- Add memory coalescing to reduce fragmentation
- Implement more sophisticated allocation strategies
- Add memory alignment support

## License

[Your license information here] 