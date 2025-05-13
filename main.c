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

void chunk_list_dump(const Chunk_List *list)
{
  printf("Allocated Chunks (%zu): \n",list->count );
  for (size_t i = 0; i < list->count; i++){
    printf("start : %p, size : %zu \n",

           list->chunks[i].start,
           list->chunks[i].size);
  }
}

int chunk_start_compare(const void *a, const void *b)
{
  const Chunk *a_chunk = a;
  const Chunk *b_chunk = b;
  return a_chunk->start - b_chunk->start;
}

int chunk_list_find(const Chunk_List *list,  void *ptr )
{
  Chunk key = {
    .start = ptr
  };
  Chunk *result = bsearch(&key, list->chunks,
                list->count, sizeof(list->chunks[0]),
                chunk_start_compare);
  if (result != 0) {
    assert(list->chunks <= result);
    return (result - list->chunks) / sizeof(list->chunks[0]);
  } else {
    return -1;
  }
} 

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


void *heap_alloc(size_t size)
{

  if (size  > 0 ) {
    for (size_t i = 0; i < freed_chunks.count; ++i) {
      if (freed_chunks.chunks[i].size >=  size) {
        const Chunk chunk = freed_chunks.chunks[i];
        chunk_list_remove(&freed_chunks, i );

        const  size_t tail_size = chunk.size - size;

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

// O(Alloced)
void heap_free(void *ptr) 
{
  if (ptr != NULL) {
    const int index = chunk_list_find(&alloced_chunk,ptr);
    assert(index >= 0);

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

int main(void)
{
  for (int i = 0; i < 10; i++) {
    void *p = heap_alloc(i);
    (void) p;
    if (i%2 == 0) {
      heap_free(p);
    }
  }

  heap_alloc(420);
  
  for (int i = 1; i <= 4; ++i ){
    heap_alloc(i);
  } 
  chunk_list_dump(&alloced_chunk);
  chunk_list_dump(&freed_chunks);
  return 0;
}
