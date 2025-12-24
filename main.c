#include <stdio.h>

#define BLOAT_IMPLEMNTATION
#include "bloat.h"

struct Foo {
    char *name;
    int age;
};

int main(void)
{
    arena_t *arena = arena_alloc();

    string_builder_t *sb = arena_push(arena, sizeof(string_builder_t));

    struct Foo *foo = arena_push(arena, sizeof(struct Foo));

    sb_append(sb, "Ur mom");
    printf("String builder: %s\n", sb->items);

    foo->name = "Hello world";
    foo->age = 20;

    printf("Foo struct: %s %d\n", foo->name, foo->age);

    free(arena);
    return 0;
}
