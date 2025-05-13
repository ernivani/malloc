# Custom Malloc Implementation

A simple custom memory allocation system implemented in C.

## Overview

This project implements a basic heap memory allocator with the following features:

- Fixed-size heap (640KB)
- First-fit allocation strategy
- Chunk-based memory management
- Allocation and deallocation functions
- Memory chunk tracking
- Automatic memory coalescing

## Functions

- `void *heap_alloc(size_t size)` - Allocates memory of specified size
- `void heap_free(void *ptr)` - Frees previously allocated memory
- `void heap_collect(void)` - Garbage collection (not implemented yet)

## Building and Running

```bash
# Build  and run the project
make all

# OR
gcc -Wall -Wextra -o heap main.c
./heap
```

## Memory Management

The allocator maintains two chunk lists:
- `alloced_chunk`: Tracks allocated memory regions
- `freed_chunks`: Tracks available memory regions

When memory is allocated:
1. The allocator first merges adjacent free chunks to reduce fragmentation
2. Searches for a free chunk large enough using first-fit strategy
3. Splits the chunk if necessary
4. The allocated portion is moved to the allocated list
5. Any remainder stays in the free list

When memory is freed:
1. The chunk is moved from the allocated list to the free list
2. Adjacent free chunks are automatically coalesced during the next allocation

## Implementation Details

- Chunks are kept sorted by memory address to facilitate coalescing
- The `chunk_list_merge` function handles both sorting and coalescing of chunks
- Time complexity for allocation is O(n) where n is the number of free chunks
- Time complexity for deallocation is O(n) where n is the number of allocated chunks

## Future Improvements

- Implement `heap_collect()` for garbage collection
- Implement more sophisticated allocation strategies (best-fit, worst-fit)
- Add memory alignment support
- Optimize chunk list operations for better performance
