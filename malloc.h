#pragma once

#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <limits.h>
#include <errno.h>
#include <pthread.h>

#define TINY_ZONE_SIZE (getpagesize() * 4)
#define TINY_BLOCK_SIZE (TINY_ZONE_SIZE / 128)
#define SMALL_ZONE_SIZE (getpagesize() * 16)
#define SMALL_BLOCK_SIZE (SMALL_ZONE_SIZE / 128)

enum e_zone_type {
    TINY,
    SMALL,
    LARGE,
};

//#define DEBUG

#ifdef DEBUG
    #define dbg(s, ...) {fprintf(stderr, "debug: "); fprintf(stderr, s, ##__VA_ARGS__);}
#else
    #define dbg(s, ...)
#endif

//#ifdef DEBUG 
    //#define dbg printf;
//#else
    //#define dbg 
//#endif

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

extern pthread_mutex_t g_mutex;
extern t_zone *g_zones[3];

void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void show_alloc_mem();
void show_alloc_mem_ex();
void *calloc(size_t nmemb, size_t size);
