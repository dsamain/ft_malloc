#pragma once

#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>

#define TINY_ZONE_SIZE (getpagesize() * 4)
#define TINY_BLOCK_SIZE (TINY_ZONE_SIZE / 128)
#define SMALL_ZONE_SIZE (getpagesize() * 16)
#define SMALL_BLOCK_SIZE (SMALL_ZONE_SIZE / 128)

enum e_zone_type {
    TINY,
    SMALL,
    LARGE
};

typedef struct s_zone {
    struct s_zone *next;
    struct s_zone *prev;
    enum e_zone_type type;
    size_t size; // size of the zone include metadata
    size_t free_size; // size of the free space at the end of the zone (excluding free blocks)
    size_t block_count;
} t_zone;

typedef struct s_block {
    struct s_block *next;
    struct s_block *prev;
    size_t size; // size of the block include metadata
    int is_free;
} t_block;


void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);