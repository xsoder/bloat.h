#ifndef BLOAT_H
#define BLOAT_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define DEFAULT_CAPACITY 1024

typedef struct {
    uint64_t pos;
    uint64_t size;
    uint64_t capacity;
    void *data;
} arena_t;

// NOTE: Functions are bellow
arena_t *arena_alloc();
void arena_free(arena_t *);

void *arena_push(arena_t *, uint64_t);
void *arena_push_zero(arena_t *, uint64_t);

void arena_pop(arena_t *, uint64_t);
void arena_pop_to(arena_t *, uint64_t);
void arena_clear(arena_t *);

#ifdef BLOAT_IMPLEMNTATION
/*
This power macro is used to see if it is a module or not
and it will return a boolean
*/
// TODO: Better name
#define IS_ALLIGN(x, y) ((x & (y-1)) == 0)

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define ARENA_BASE_POS (sizeof(arena_t))

#define TODO(x) {      \
    printf("%s\n", x); \
    abort();          \
}

uint64_t align_mem(uint64_t pos, uint64_t size)
{
    if (IS_ALLIGN(pos, size)) return pos;
    return ((pos + (size - 1)) & ~(size - 1));
}

arena_t *arena_alloc()
{
    arena_t *arena = malloc(sizeof(arena_t));
    arena->capacity = DEFAULT_CAPACITY;
    arena->size = 0;
    arena->pos = 0;
    arena->data = malloc(sizeof(arena->size) * arena->capacity);
    if (!arena->data) {
        free(arena);
        return NULL;
    }
    return arena;
}

void arena_free(arena_t *arena)
{
    free(arena->data);
    free(arena);
}

void *arena_push(arena_t *arena, uint64_t size)
{
    // TODO: Add proper enum for error handling
    uint64_t _pos = align_mem(arena->pos, size);
    uint64_t _new_pos = _pos + size;

    if (_new_pos > arena->capacity) {
        return NULL;
    }

    arena->pos = _new_pos;
    return (char*)arena->data + _pos;
}

void *arena_push_zero(arena_t *arena, uint64_t size)
{
    void *ptr = arena_push(arena, size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

void arena_pop(arena_t *arena, uint64_t size)
{
    size = MIN(size, arena->pos - ARENA_BASE_POS);
    arena->pos -= size;
}

void arena_pop_to(arena_t *arena, uint64_t pos)
{
    if ( pos >= arena->pos) return;
    uint64_t _size = pos - arena->pos;
    arena_pop(arena, _size);
}

void arena_clear(arena_t *arena)
{
    arena_pop_to(arena, ARENA_BASE_POS);
}

#endif
#endif //BLOAT_H
