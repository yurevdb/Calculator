#include <stdlib.h>

typedef struct {
  char *region;
  size_t index;
  size_t size;
} Arena;

Arena* arena_create(size_t size);
void* arena_alloc(Arena *arena, size_t size);
void arena_clear(Arena* arena);
void arena_destroy(Arena *arena);
