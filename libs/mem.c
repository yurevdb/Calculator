#include "mem.h"
#include "stdint.h"

Arena* arena_create(size_t size) {
  Arena *arena;

  if (size == 0) {
    return NULL;
  }

  arena = malloc(sizeof(Arena));
  if (arena == NULL) {
    return NULL;
  }

  arena->region = malloc(size);
  if (arena->region == NULL) {
    free(arena);
    return NULL;
  }

  arena->index = 0;
  arena->size = size;

  return arena;
}

void* arena_alloc(Arena *arena, size_t size) {
  unsigned int offset;

  if (size == 0) {
    return NULL;
  }

  if (arena == NULL || arena->region == NULL) {
    return NULL;
  }

  offset = 0;

  if (arena->size - (arena->index + offset) < size) {
    return NULL;
  }

  arena->index += size;
  return arena->region + (arena->index - size);
}

void arena_clear(Arena *arena) {
  if (arena == NULL) {
    return;
  }

  arena->index = 0;
}

void arena_destroy(Arena *arena) {
  if (arena == NULL) {
    return;
  }

  if (arena->region != NULL) {
    free(arena->region);
  }

  free(arena);
}
