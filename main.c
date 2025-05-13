#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define HEAP_CAP 640000
#define CHUNK_LIST_CAP  1024

#define UNIMPLEMENTED() \
  do { \
    fprintf(stderr, "%s:%d: TODO : %s is not implemented yet\n", \
            __FILE__, __LINE__, __func__); \
    abort(); \
  } while(0)

typedef struct {
  char *start;
  size_t size;
} Chunk;

typedef struct {
  size_t count;
  Chunk chunks[CHUNK_LIST_CAP];
} Chunk_List;


// O(n) n number of chunks in the list
void chunk_list_insert(Chunk_List *list, void *start, size_t size) 
{

  assert(list->count < CHUNK_LIST_CAP);

  list->chunks[list->count].start = start;
  list->chunks[list->count].size = size;
  
  for (size_t i = list->count;
        i > 0 && list->chunks[i].start < list->chunks[i - 1].start;
        --i ) {
    const Chunk t = list->chunks[i];
    list->chunks[i] = list->chunks[i - 1];
    list->chunks[i - 1] = t;
  }

  list->count += 1;

}

// O(n²) n  number of chunks in src
void chunk_list_merge(Chunk_List *dst, const Chunk_List *src)
{
  dst->count = 0;
  
  for (size_t i = 0; i < src->count; ++i) {
    chunk_list_insert(dst, src->chunks[i].start, src->chunks[i].size);
  }
  
  if (dst->count <= 1) {
    return;
  }
  
  size_t write_idx = 0;
  
  for (size_t read_idx = 1; read_idx < dst->count; ++read_idx) {
    Chunk *current = &dst->chunks[write_idx];
    Chunk *next = &dst->chunks[read_idx];
    
    if (current->start + current->size == next->start) {
      current->size += next->size;
    } else {
      write_idx++;
      if (write_idx != read_idx) {
        dst->chunks[write_idx] = dst->chunks[read_idx];
      }
    }
  }
  
  dst->count = write_idx + 1;
}


// O(n) n  number of chunks in the list
void chunk_list_dump(const Chunk_List *list)
{
  printf("Allocated Chunks (%zu): \n", list->count);
  for (size_t i = 0; i < list->count; i++) {
    printf("start : %p, size : %zu \n",
           (void*)list->chunks[i].start,
           list->chunks[i].size);
  }
}


// O(n) n number of chunks in the list
int chunk_list_find(const Chunk_List *list,  void *ptr )
{

  for (size_t i = 0; i < list->count; ++i) {
    if (list->chunks[i].start == ptr) {
      return (int) i;
    }
  }
  return -1;
} 

// O(n) n number of chunks in the list
void chunk_list_remove(Chunk_List *list, size_t index) 
{
  assert(index < list->count);

  for (size_t i = index; i < list->count - 1; ++i) {
    list->chunks[i] = list->chunks[i+1];
  }
  list->count -= 1;
}

char heap[HEAP_CAP] = {0};

Chunk_List alloced_chunk =  {0};
Chunk_List freed_chunks =  {
  .count = 1,
  .chunks = {
      [0] = {.start = heap, .size = sizeof(heap)}
  },
};
Chunk_List tmp_chunk = {0};

// O(n) n number of free chunks
void *heap_alloc(size_t size)
{
  if (size > 0) {
    tmp_chunk.count = 0;

    chunk_list_merge(&tmp_chunk, &freed_chunks);
    freed_chunks = tmp_chunk;

    for (size_t i = 0; i < freed_chunks.count; ++i) {
      if (freed_chunks.chunks[i].size >= size) {
        const Chunk chunk = freed_chunks.chunks[i];
        chunk_list_remove(&freed_chunks, i);

        const size_t tail_size = chunk.size - size;

        chunk_list_insert(&alloced_chunk, chunk.start, size);
        if (tail_size > 0) {
          chunk_list_insert(&freed_chunks, chunk.start + size, tail_size);
        }

        return chunk.start;
      }
    }
  }

  return NULL;
}

// O(n) n number of allocated chunks
void heap_free(void *ptr) 
{
  if (ptr != NULL) {
    const int index = chunk_list_find(&alloced_chunk,ptr);
    assert(index >= 0);
    assert(ptr == alloced_chunk.chunks[index].start);
    chunk_list_insert(&freed_chunks,
                      alloced_chunk.chunks[index].start,
                      alloced_chunk.chunks[index].size);
    chunk_list_remove(&alloced_chunk, (size_t) index);
  }
}

void heap_collect(void) 
{
  UNIMPLEMENTED();
}

#define N 10

void *ptrs[N] = {0};

int main(void)
{
  // for (int i = 0; i < 10; i++) {
  //   void *ptrs = heap_alloc(i);
    // if (i%2 == 0) {
      // heap_free(ptrs);
    // }
  // }

  // heap_alloc(420);
  //
  // for (int i = 1; i <= 4; ++i ){
  //   heap_alloc(i);
  // }

  
  long long *ptr;
  ptr = heap_alloc( sizeof(*ptr)); 
  
 // if (ptr != NULL) {
 //    *(ptr + 5) = 480;
 //    printf("Value of the 6th integer is %d",*(ptr + 5));
 //  }

  chunk_list_dump(&alloced_chunk);

  printf("Free blocks:\n");
  chunk_list_dump(&freed_chunks);
  
  return 0;
}
